/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 16:59:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

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
				join.ship();
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

/*MODE*/

void Client::handleMode( t_arg args )
{
	Reply	mode(*this);
	
	if (args.size() == 1)
		mode.ship(403);
	else if (_server->isChannelExist(args[1]) == false)
		mode.ship(403);
	else 
	{
		if (args.size() == 2)
		{
			Reply	rpl_channelmodeis(*this, _nickname);
			Channel	*chan = _server->getChannel(args[1]);;
			
			rpl_channelmodeis.list(args[1]);
			rpl_channelmodeis.list("InviteOnly");
			rpl_channelmodeis.list(chan->isInviteOnly());
			rpl_channelmodeis.ship(324);
			rpl_channelmodeis.list("Test");
			rpl_channelmodeis.list("TopicLocked");
			rpl_channelmodeis.list(chan->isTopicLocked());
			rpl_channelmodeis.ship(324);
			rpl_channelmodeis.list(args[1]);
			rpl_channelmodeis.list("HasPassword");
			rpl_channelmodeis.list(chan->hasKey());
			rpl_channelmodeis.ship(324);
			rpl_channelmodeis.list(args[1]);
			rpl_channelmodeis.list("HasLimit");
			rpl_channelmodeis.list(chan->getLimit());
			rpl_channelmodeis.ship(324);
		}
		else
		{
		}

	}
	return;
}

/*void Client::handleTopic( t_args args )
{

	return ;
}*/
