/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:42:42 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 13:04:51 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/utils.hpp"

/*PRIVMSG*/
void Client::handlePrivMsg( t_arg args )
{
	Reply	privmsg(*this);
	std::map<int, std::string>	targets;
	std::vector<std::string>	tmp;	
	
	if (args.size() != 2) //needs at least a target and a message to send
		privmsg.ship(412);
	tmp = split(args[0], ',');
	for (size_t i = 0; i < tmp.size(); i++)
	{
		size_t j = tmp[i].find("#");
		if (j != std::string::npos)
		{
			std::string chan = tmp[i].substr(j, tmp[i].size());
			if (_server->isChannelExist(chan) == false)
				privmsg.ship(403);
			else if (j > 1 && tmp[i][0] == '&')
				targets.insert(std::pair<int, std::string>(2, chan));
			else if (j > 1 && tmp[i][0] != '&')
				privmsg.ship(403);
			else
				targets.insert(std::pair<int, std::string>(1, chan));
		}
		else
		{
			if (_server->isClientExist(tmp[i]) == true)
				targets.insert(std::pair<int, std::string>(0, tmp[i]));
			else
				privmsg.ship(401);
		}
	}
	
	std::map<int, std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); it++)
	{
		switch (it->first) {
			case 0:
			{
				if (_away == true)
					privmsg.ship(301);
				privmsg.ship(args[1]);
				break;
			}
			case 1:
			{
				_server->getChannel(it->second)->broadcast_all(args[1], getNickname());
				break;
			}
			case 2:
			{
				_server->getChannel(it->second)->broadcast_ops(args[1], getNickname());
				break;
			}
		}	
	}
	return;
}
