/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/09 16:29:41 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"

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
				Reply	join(*this, _nickname, *c, 'a', 'n');
				join.list(args[0]);
				join.list(it->first);
				join.ship();
				if (c->getTopic().empty() == false)
					rpl_Topic(*c);
				rpl_NamReply(*c);
				rpl_EndOfNames(*c);
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
				rpl_NamReply(*c);
				rpl_EndOfNames(*c);
		}
	}
	return;
}

/*KICK*/
void Client::handleKick( t_arg args )
{
	std::vector<std::string>					user_list;
	std::vector<std::string>::const_iterator	it;
	
	if (args.size() < 3 || args.size() > 4)
		return (err_NeedMoreParameters(args[0]));
	if (_server->isChannelExist(args[1]) == false)
		return (err_noSuchChannel(args[1]));
	if (args.size() == 3)
		   args.insert(std::pair<int, std::string>(4, "No reason provided"));
	user_list = split(args[2], ',');
	if (static_cast<int>(user_list.size()) > std::atoi(_server->getSetting("TARGMAX").c_str()))
		return ;	
	Channel *chan = _server->getChannel(args[1]);
	for (it = user_list.begin(); it != user_list.end(); it++)
	{
		if (_server->isClientExist(*it) == false)
			err_noSuchNick();
		else if (chan->isMember(_server->getClient(*it)) == false)
			err_notOnChannel(args[1]);
		else if (chan->isOperator(*this) == false)
			err_ChanOPrivsNeeded(args[1]);
		else
		{
			Reply	kick(*this, _nickname, *chan, 'a', 'n');
			kick.list("KICK");
			kick.list(args[1]);
			kick.list(*it);
			kick.list(args[3]);
			kick.ship();
			chan->removeMember(_server->getClient(*it));
		}
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
		return (err_UserOnChannel(args[2]));
	if (chan->isMember(*this) == false)
		return (err_notOnChannel(args[2]));
	if (chan->isInviteOnly() == true && chan->isOperator(*this) == false)
		return (err_ChanOPrivsNeeded(args[2]));
	chan->addInvite(_server->getClient(args[1]));
	rpl_Inviting(args[2], args[1]);
	Reply	invite(*this, _nickname);
	invite.list("INVITE");
	invite.list(args[1]);
	invite.list(args[2]);
	invite.ship();
	return;
}

/*TOPIC*/
void	Client::handleTopic( t_arg args )
{
	if (args.size() < 2 || args.size() > 3)
		return (err_NeedMoreParameters(args[0]));
	if (_server->isChannelExist(args[1]) == false)
		return (err_noSuchChannel(args[1]));
	Channel *chan = _server->getChannel(args[1]);
	if (chan->isMember(*this) == false)
		return (err_notOnChannel(args[1]));
	if (chan->isTopicLocked() == true && chan->isOperator(*this) == false)
		return (err_ChanOPrivsNeeded(args[1]));
	if (args.size() == 2)
		return (rpl_noTopic(args[1]));
	if (args[2].empty() == true)
		return (rpl_noTopic(args[1]));
	else
	{
		chan->setTopic(args[2]);
		rpl_TopicAll(args[1]);
	}
	return ;
}
