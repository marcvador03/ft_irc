/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChanOps.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:41:55 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/17 14:46:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

/*JOIN*/
void Client::handleJoin( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
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
		reply(461);
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
		cmd_reply.push_back(args[0]);
		cmd_reply.push_back(it->first);
		switch (joinChannel(it->first, it->second)) {
			case 405:
				reply(405);
				break ;
			case 475:
				reply(475);
				break ;
			case 471:
				reply(471);
				break ;
			case 473:
				reply(473);
				break ;
			case 476:
				reply(476);
				break ;
			case 0:
				reply(_nickname, cmd_reply);
				reply(332);
				reply(333);
		}
	}
	return;
}

/*PART*/
void Client::handlePart( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	t_list				list;
	t_list::iterator 	it;
	std::istringstream	chan_s(args[1]);
	std::string			chan;
	
	if (args.size() < 2 || args[1].empty()) 
	{
		reply(461);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
		list.insert(std::make_pair(chan, args[2]));
	for (it = list.begin(); it!= list.end(); it++)
	{
		switch (leaveChannel(it->first)) {
			case 403:
				reply(403);
				break ;
			case 442:
				reply(442);
				break ;
			case 0:
				cmd_reply.push_back(args[0]);
				cmd_reply.push_back(it->first);
				reply(cmd_reply);
		}
	}
	return;
}

/*void Client::handleTopic( t_args args )
{

	return ;
}*/
