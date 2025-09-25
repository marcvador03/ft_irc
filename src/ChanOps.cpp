/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/23 18:04:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
/*RPL_NAMES_REPLY*/
void	Client::rpl_NamReply( Channel &chan )
{
	Reply	rpl(*this);
	std::map<int, Client *>::const_iterator it;

	for (it = chan.getAllClients().begin(); it != chan.getAllClients().end(); it++)
	{
		rpl.list(_nickname);
		rpl.list("=");
		rpl.list(chan.getName());
		if (chan.isOperator(it->second) == true)
			rpl.list("@" + it->second->getNickname());
		else
			rpl.list(it->second->getNickname());
		rpl.ship(353);
	}
	rpl_EndOfNames(chan);	
}

void	Client::rpl_EndOfNames( Channel &chan )
{
	Reply	rpl(*this);

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

/*JOIN*/
void Client::handleJoin( t_arg args ) 
{
	Reply				join(*this, _nickname);
	t_list				list;
	t_list::iterator	it;
	std::string			chan, key;
	
	std::istringstream chan_s(args[1]);
	std::istringstream key_s(args[2]);
	
	//special case: "JOIN 0" means leave all channels
	if (args[1] == "0")
	{
		leaveAllChannels();	
		return ;
	}
	if (args.size() < 2 || args[1].empty()) 
	{
		join.ship(461);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
	{
		std::getline(key_s, key, ',');
		log("Channel:" + chan + " Key:" + key);
		list.insert(std::make_pair(chan, key));
	}
	for (it = list.begin(); it!= list.end(); it++)
	{
		join.list(args[0]);
		join.list(it->first);
		switch (joinChannel(it->first, it->second)) {
			case 405:
				join.ship(405);
				break ;
			case 475:
				join.ship(475);
				break ;
			case 471:
				join.ship(471);
				break ;
			case 473:
				join.ship(473);
				break ;
			case 476:
				join.ship(476);
				break ;
			case 0:
			{
				join.ship();
				Channel *c = _server->getChannel(it->first, this);
				if (c->getTopic().empty() == false)
					rpl_Topic(*c);
				c->broadcast_all(this, Client::rpl_NamReply(*c), *c);
			}
		}
	}
	return;
}

/*PART*/
void Client::handlePart( t_arg args ) 
{
	Reply				part(*this);
	t_list				list;
	t_list::iterator 	it;
	std::istringstream	chan_s(args[1]);
	std::string			chan;
	
	if (args.size() < 2 || args[1].empty()) 
	{
		part.ship(461);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
		list.insert(std::make_pair(chan, args[2]));
	for (it = list.begin(); it!= list.end(); it++)
	{
		switch (leaveChannel(it->first)) {
			case 403:
				part.ship(403);
				break ;
			case 442:
				part.ship(442);
				break ;
			case 0:
				part.list(args[0]);
				part.list(it->first);
				part.ship();
		}
	}
	return;
}


/*void Client::handleTopic( t_args args )
{

	return ;
}*/
