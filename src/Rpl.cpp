/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:01:53 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/22 14:11:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*RPL_AWAY 301*/
void	Client::rpl_Away( const std::string &nick )
{
	Reply	rpl(*this);

	if (_server->getClient(nick).getAwayStatus() == false)
		return;
	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list(_server->getClient(nick).getAwayMsg());
	rpl.ship(301);
	return ;
}

/*305*/
void	Client::rpl_Unaway( void )
{
	Reply	rpl(*this);

	rpl.list(_nickname);
	rpl.list("You are no longer marked as being away");	
	rpl.ship(305);

	return;
}

/*306*/
void	Client::rpl_NowAway( void )
{
	Reply	rpl(*this);
	
	rpl.list(_nickname);
	rpl.list("You have been marked as being away");	
	rpl.ship(306);

	return;
}

/*RPL_NAMES_REPLY*/
void	Client::rpl_NamReply( Channel &chan )
{
	Reply	rpl(*this, chan, 'a', 'n');
	std::map<int, Client *>::const_iterator it;
	std::string	list;

	rpl.list(_nickname);
	rpl.list("=");
	rpl.list(chan.getName());
	for (it = chan.getAllClients().begin(); it != chan.getAllClients().end(); it++)
	{
		if (chan.isOperator(*it->second) == true)
			list += "@";
		list += it->second->getNickname();
		if ((it != chan.getAllClients().end()) && (it != --chan.getAllClients().end()))
			list += " ";
	}
	rpl.list(list);
	rpl.ship(353);
}

void	Client::rpl_EndOfNames( Channel &chan )
{
	Reply	rpl(*this, chan, 'a', 'n');

	rpl.list(_nickname);
	rpl.list(chan.getName());
	rpl.list("End of /NAMES list");
	rpl.ship(366);
}

/*RPL_TOPIC*/
void	Client::rpl_noTopic( Channel &chan )
{
	Reply	rpl(*this);

	rpl.list(_nickname);
	rpl.list(chan.getName());
	rpl.list("No topic is set");
	rpl.ship(331);
	return ;
}

void	Client::rpl_Topic( Channel &chan )
{
	Reply	rpl(*this);

	rpl.list(_nickname);
	rpl.list(chan.getName());
	rpl.list(chan.getTopic());
	rpl.ship(332);
	return;
}

void	Client::rpl_ChannelModeIs( Channel &chan, const std::string &target)
{
	Reply	rpl(*this);

	std::string keys;
	std::map<int, Client *>::const_iterator it;
	
	keys = "+";
	for (it = chan.getOpsClients().begin(); it != chan.getOpsClients().end(); it++)
		keys += "o";
	if (chan.hasKey() == true)
		keys += "k";
	if (chan.getLimit() > 0)
		keys += "l";
	if (chan.isInviteOnly() == true)
		keys += "i";
	if (chan.isTopicLocked() == true)
		keys += "k";
	rpl.list("marc");
	rpl.list(target);
	rpl.list(keys);
	for (it = chan.getOpsClients().begin(); it != chan.getOpsClients().end(); it++)
		rpl.list(it->second->getNickname());
	if (chan.hasKey() == true)
		rpl.list(chan.getKey());
	if (chan.getLimit() > 0)
		rpl.list(chan.getLimit());
	rpl.ship(324);
	return;
}

void	Client::rpl_CreationTime( const std::string &target )
{
	Reply	rpl(*this);
	
	rpl.list(target);
	//rpl.list(chan->getCreationTime());
	rpl.ship(329);
	return;
}

void	Client::rpl_Welcome( void )
{
	Reply	rpl(*this);
	
	rpl.list(_nickname); // client is not nickname, to be checked
	rpl.list("Welcome to the network, " + _nickname + "!" + _username + "@" + _host);
	rpl.ship(1);
	return;
}

void	Client::rpl_YourHost( void )
{
	Reply	rpl(*this);
	
	rpl.list(_nickname); // client is not nickname, to be checked
	rpl.list("Your host is ");
	rpl.list(getServername());
	rpl.ship(2);
	return;
}

void	Client::rpl_Created( void )
{
	Reply	rpl(*this);
	
	rpl.list(getNickname()); // client is not nickname, to be checked
	rpl.list("The server was created ");
	rpl.list(getServerLaunchTime());
	rpl.list(", running version ");
	rpl.list(getServerVersion());
	rpl.ship(3);
	return;
}

void	Client::rpl_MyInfo( void )
{
	Reply	rpl(*this);

	rpl.list(getNickname()); // client is not nickname, to be checked
	rpl.list(getServername());
	rpl.list(getServerVersion());
	rpl.list("o");
	rpl.list("itlbk");
	rpl.ship(4);
	return;
}

void	Client::rpl_ISupport( void )
{
	Reply	rpl(*this);
	t_settings	settings = getServerSettings();
	for (size_t i = 0; i < settings.size(); i++)
	{
			rpl.list(getNickname()); // client is not nickname, to be checked
			for (t_list::const_iterator it = settings[i].begin(); it != settings[i].end(); it++)	
				rpl.list(it->first + "=" + it->second);
			rpl.list("are supported by this server");
			rpl.ship(5);
	}
	return;
}

/*RPL_INVITING*/
void	Client::rpl_Inviting( const std::string &chan, const std::string &inv )
{
	Reply	rpl(*this, _nickname);

	rpl.list(_nickname);
	rpl.list(inv);
	rpl.list(chan);
	rpl.ship(341);
	return;
}

/*RPL_NOTOPIC*/
void	Client::rpl_noTopic( const std::string &chan )
{
	Reply	rpl(*this, *_server->getChannel(chan), 'a', 'n');

	rpl.list(_nickname);
	rpl.list(chan);
	rpl.list("No topic is set");
	rpl.ship(331);
	
	return;
}

/*RPL_TOPIC*/
void	Client::rpl_TopicAll( const std::string &chan )
{
	Reply	rpl(*this, *_server->getChannel(chan), 'a', 'n');

	rpl.list(_nickname);
	rpl.list(chan);
	rpl.list(_server->getChannel(chan)->getTopic());
	rpl.ship(332);
		
	return;
}

void	Client::rpl_Topic( const std::string &chan )
{
	Reply	rpl(*this);

	rpl.list(_nickname);
	rpl.list(chan);
	rpl.list(_server->getChannel(chan)->getTopic());
	rpl.ship(332);
		
	return;
}

void	Client::rpl_UnexpectedQuit( const std::string &msg )
{
	Reply	rpl(*this);

	/*rpl.list("ERROR");
	rpl.list(msg);
	rpl.ship();*/
	rpl.list("QUIT");
	rpl.list(msg);
	rpl.ship();
	
	return;
}


/*352*/
void	Client::rpl_WhoReply( const std::string &chan )
{
	Reply		rpl(*this);
	std::string	str;

	rpl.list(_nickname);
	rpl.list(chan);
	rpl.list(_username);
	rpl.list(_host);
	rpl.list(getServername());
	rpl.list(_nickname);
	if (_away == false)
		str = "H";
	else
		str = "G";
	if (_server->isChannelExist(chan) == true && _server->getChannel(chan)->isOperator(*this) == true)
		str += "*";
	rpl.list(str);
	rpl.list("0" + _realname);
	rpl.ship(352);
	return;
}

/*315*/
void	Client::rpl_EndOfWho( const std::string &mask )
{
	Reply		rpl(*this);

	rpl.list(_nickname);
	rpl.list(mask);
	rpl.ship(315);
	return ;
}

/*311*/
void	Client::rpl_WhoIsUser( const std::string &nick )
{
	Reply		rpl(*this);

	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list(_server->getClient(nick).getUser());
	rpl.list(_server->getClient(nick).getHost());
	rpl.list("*");
	rpl.list(_server->getClient(nick).getRealname());
	rpl.ship(311);	
	
	return;
}

/*312*/
void	Client::rpl_WhoIsServer( const std::string &nick )
{
	Reply		rpl(*this);
	
	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list(getServername());
	rpl.list("FT_IRC server");
	rpl.ship(312);
	
	return;
}

/*319*/
void	Client::rpl_WhoIsChannels( const std::string &nick )
{
	Reply								rpl(*this);
	std::vector<Channel *>::iterator	it;
	std::vector<Channel *>				chan_list;

	chan_list = _server->getChannelsforClient(_server->getClient(nick));
	if (chan_list.empty() == true)
		return;
	rpl.list(_nickname);
	rpl.list(nick);
	for (it = chan_list.begin(); it != chan_list.end(); it++)
	{
		if ((*it)->isOperator(_server->getClient(nick)) == true)
			rpl.list("@" + (*it)->getName());
		else
			rpl.list((*it)->getName());
	}
	rpl.ship(319);
	
	return;
}

/*338*/
void	Client::rpl_WhoIsActually( const std::string &nick )
{
	Reply		rpl(*this);

	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list(_username + "@" + _host);
	rpl.list("is actually using host");
	rpl.ship(338);
	
	return;
}

/*378*/
void	Client::rpl_WhoIsHost( const std::string &nick )
{
	Reply		rpl(*this);
	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list("is connecting from " + _host);
	rpl.ship(378);
	
	return;
}

/*318*/
void	Client::rpl_EndofWhoIs( const std::string &nick )
{
	Reply		rpl(*this);
	
	rpl.list(_nickname);
	rpl.list(nick);
	rpl.list("End of /WHOIS list");
	rpl.ship(318);

	return;
}

/*321*/
void	Client::rpl_ListStart( void )
{
	Reply		rpl(*this);
	
	rpl.list(_nickname);
	rpl.list("Channel :Users Name");
	rpl.ship(321);
	
	return;
}

/*322*/
void	Client::rpl_List( Channel & chan)
{
	Reply		rpl(*this);
	std::map<int, Client *> list;
	std::ostringstream	ss;
	
	rpl.list(_nickname);
	rpl.list(chan.getName());
	list = chan.getAllClients();
	ss << list.size();
	rpl.list(ss.str());
	rpl.list(chan.getTopic());
	rpl.ship(322);
	
	return;
}

/*323*/
void	Client::rpl_ListEnd( void )
{
	Reply		rpl(*this);
	
	rpl.list(_nickname);
	rpl.list("End of /LIST");
	rpl.ship(323);
	
	return;
}
