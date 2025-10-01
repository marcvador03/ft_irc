/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/01 15:39:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
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
		leaveAllChannels();	
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
		log("Channel:" + chan + " Key:" + key);
		list.insert(std::make_pair(chan, key));
	}
	for (it = list.begin(); it!= list.end(); it++)
	{
		Channel *c = _server->getChannel(it->first, this);
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
	Reply				part(*this);
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
		}
	}
	return;
}


/*void Client::handleTopic( t_args args )
{

	return ;
}*/
