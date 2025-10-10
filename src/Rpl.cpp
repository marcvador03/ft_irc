/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:01:53 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/08 16:35:18 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*RPL_AWAY*/
void	Client::rpl_Away( void )
{
	Reply	rpl(*this);

	rpl.list(_nickname);
	rpl.list(_nickname);
	//client away message string
	rpl.ship(301);
	return ;

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
