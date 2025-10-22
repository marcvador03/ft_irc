/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/22 14:20:51 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Utils.hpp"

/*JOIN*/
void Client::handleJoin( t_arg args ) 
{
	t_list				list;
	t_list::iterator	it;
	std::string			chan, key;
	
	std::istringstream chan_s(args[1]);
	std::istringstream key_s(args[2]);
	
	//special case: "JOIN 0" means leave all channels
	if (args[1] == "0")
	{
		std::set<Channel *>::iterator it2;
		std::string tmp = "";
		t_arg args2;
		args2.insert(std::pair<int, std::string>(0, "PART"));
		for (it2 = this->_channels.begin(); it2 != this->_channels.end() ; it2++)
			tmp += (*it2)->getName() + ",";
		tmp.erase(tmp.size() - 1, 1);
		args2.insert(std::pair<int, std::string>(1, tmp));
		handlePart(args2);	
		return ;
	}
	if (args.size() < 2 || args[1].empty()) 
	{
		err_NeedMoreParameters(args[0]);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
	{
		std::getline(key_s, key, ',');
		list.insert(std::make_pair(chan, key));
	}
	for (it = list.begin(); it!= list.end(); it++)
	{
		Channel *c = _server->getChannel(it->first, *this);
		switch (joinChannel(it->first, it->second)) {
			case 405:
				err_tooManyChannels(it->first);
				break ;
			case 475:
				err_BadChannelKey(it->first);
				break ;
			case 471:
				err_ChannelIsFull(it->first);
				break ;
			case 473:
				err_InviteOnlyChan(it->first);
				break ;
			case 476:
				err_BadChanMask(it->first);
				break ;
			case 0:
			{
				// full user mask is: nick!user@host
				std::string fullsrc = _nickname;
				if (!getUser().empty() && !getHost().empty())
					fullsrc += "!" + getUser() + "@" + getHost();
				
				// broadcast JOIN (audience 'a', do not skip sender 'n')
				Reply	join(*this, fullsrc, *c, 'a', 'n');
				join.list("JOIN");
				join.list(c->getName()); //channel name
				join.ship();
				if (c->getTopic().empty() == false) 
					rpl_Topic(*c); //sends 332
				else
					rpl_noTopic(*c); 
				rpl_NamReply(*c); //sends 353
				rpl_EndOfNames(*c); //sends 366
			}
		}
	}
	return;
}

/*PART*/
void Client::handlePart( t_arg args ) 
{
	Reply				part(*this, _nickname);
	t_list				list;
	t_list::iterator 	it;
	std::istringstream	chan_s(args[1]);
	std::string			chan;
	
	if (args.size() < 2 || args[1].empty()) 
	{
		err_NeedMoreParameters(args[0]);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
		list.insert(std::make_pair(chan, args[2]));
	for (it = list.begin(); it!= list.end(); it++)
	{
		switch (leaveChannel(it->first)) {
			case 403:
				err_noSuchChannel(it->first);
				break ;
			case 442:
				err_notOnChannel(it->first);
				break ;
			case 0:
				part.list(args[0]);
				part.list(it->first);
				part.ship();
				Channel *c = _server->getChannel(chan);
				if (c != NULL)
				{
					rpl_NamReply(*c);
					rpl_EndOfNames(*c);
				}
		}
	}
	return;
}

/*KICK*/
void Client::handleKick( t_arg args )
{
	std::vector<std::string>			user_list;
	std::vector<std::string>::iterator	it;
	
	//KICK <channel> <user[,user2...]> [reason]
	if (args.size() < 3 || args.size() > 4)
		return (err_NeedMoreParameters(args[0])); //461
	if (_server->isChannelExist(args[1]) == false)
		return (err_noSuchChannel(args[1])); //403

	Channel *chan = _server->getChannel(args[1]);
	if (chan == NULL)
		return (err_noSuchChannel(args[1])); //403 //<-----FINISHED HERE
	
	//Kicker must be on the channel
	if (chan->isMember(*this) == false)
		return (err_notOnChannel(args[1]));

	//Kicker must have op privilieges
	if (chan->isOperator(*this) == false)
		return (err_ChanOPrivsNeeded(args[1]));

	//Reason: default if missing
	std::string reason = (args.size() >= 4 ? args[3] : std::string("for no reason"));

	//enforce KICKLEN (truncate)
	int kickLen = _server->getLen("KICKLEN", "Kick", 255);
	if (reason.length() > static_cast<size_t>(kickLen))
		reason.resize(kickLen);

	//targets
	user_list = split(args[2], ',');
	size_t targetLimit = _server->getTargmax("KICK");
	
	//enforce TARGMAX
	if (user_list.size() > targetLimit && targetLimit != 0)
		user_list.resize(targetLimit);
	
	for (it = user_list.begin(); it != user_list.end(); ++it)
	{
		if (_server->isClientExist(*it) == false)
		{
			err_noSuchNick();
			continue;
		}
		else if (chan->isMember(_server->getClient(*it)) == false)
		{
			err_UserNotInChannel(*it, args[1]);
			continue;
		}

		//broadcast KICK message to channel members
		Reply	kick(*this, _nickname, *chan, 'a', 'n');
		kick.list("KICK");
		kick.list(args[1]); //channel name
		kick.list(*it); //kicked user
		kick.list(reason); //reason
		kick.ship(); //broadcast to channel members
		chan->removeMember(_server->getClient(*it)); //remove user from channel
	}	
	return;
}

/*INVITE*/
void Client::handleInvite( t_arg args )
{
	if (args.size() < 3 || args.size() > 4)
		return (err_NeedMoreParameters(args[0]));
	if (_server->isChannelExist(args[2]) == false)
		return (err_noSuchChannel(args[2]));
	Channel *chan = _server->getChannel(args[2]);
	if (_server->isClientExist(args[1]) == false)
		return(err_noSuchNick());
	if (chan->isMember(_server->getClient(args[1])) == true)
		return (err_UserOnChannel(args[1], args[2]));
	if (chan->isMember(*this) == false)
		return (err_notOnChannel(args[2]));
	if (chan->isOperator(*this) == false)
		return (err_ChanOPrivsNeeded(args[2]));
	chan->addInvite(_server->getClient(args[1]));
	rpl_Inviting(args[2], args[1]);
	Reply	invite(*this, _nickname, _server->getClient(args[1]));
	invite.list("INVITE");
	invite.list(args[1]);
	invite.list(args[2]);
	invite.ship();
	return;
}

/*TOPIC*/
void	Client::handleTopic( t_arg args )
{
	//check if there is at least channel name
	if (args.size() < 2)
		return (err_NeedMoreParameters(args[0])); //461

	//check if channel exists
	if (_server->isChannelExist(args[1]) == false)
		return (err_noSuchChannel(args[1])); //403

	Channel *chan = _server->getChannel(args[1]);
	if (chan == NULL)
		return (err_noSuchChannel(args[1])); //403
	
	//check if user is on the channel
	if (chan->isMember(*this) == false)
		return (err_notOnChannel(args[1]));	//442

	//check if there is only channel name (no topic) - then just return the topic
	if (args.size() == 2)
	{
		if (chan->getTopic().empty())
			return (rpl_noTopic(args[1])); //331
		return (rpl_Topic(args[1])); //332
	}

	//check if topic is locked and if user is not an operator
	//if topic is locked and user is not an operator - return error
	if (chan->isTopicLocked() == true && chan->isOperator(*this) == false)
		return (err_ChanOPrivsNeeded(args[1]));

	//set new topic
	std::string newTopic = args[2];

	//if topic starts with ':', remove it
	if (newTopic[0] == ':')
		newTopic.erase(0, 1);

	//truncate topic if it's too long
	int topicLen = _server->getLen("TOPICLEN", "Topic", 307);
	if (newTopic.length() > static_cast<size_t>(topicLen))
		newTopic.resize(topicLen);

	//set the topic
	chan->setTopic(newTopic, _nickname);
	
	//broadcast to all channel members 
	rpl_TopicAll(args[1]);
	return;
}

/*LIST*/
void	Client::handleList( t_arg args )
{
	std::map<std::string, Channel *>					list;
	std::map<std::string, Channel *>::const_iterator	it;
	
	if (args.empty() == true)
		return ;
	rpl_ListStart();
	list = _server->getAllChannels();
	for (it = list.begin(); it!= list.end(); it++)
		rpl_List(*it->second);
	rpl_ListEnd();
	return ;
}
