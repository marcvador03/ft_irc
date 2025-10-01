/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:42:42 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/01 16:47:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/utils.hpp"

/*PRIVMSG*/
void Client::handlePrivMsg( t_arg args )
{
	std::map<int, std::string>	targets;
	std::vector<std::string>	tmp;	
	
	if (args.size() != 3) //needs at least a target and a message to send
		return (err_noTextToSend());
	tmp = split(args[1], ',');
	for (size_t i = 0; i < tmp.size(); i++)
	{
		size_t j = tmp[i].find("#");
		if (j != std::string::npos)
		{
			std::string chan = tmp[i].substr(j, tmp[i].size());
			if (_server->isChannelExist(chan) == false)
				err_noSuchChannel(chan);
			else if (j > 1 && tmp[i][0] == '&')
				targets.insert(std::pair<int, std::string>(2, chan));
			else if (j > 1 && tmp[i][0] != '&')
				err_noSuchChannel(chan);
			else
				targets.insert(std::pair<int, std::string>(1, chan));
		}
		else
		{
			if (_server->isClientExist(tmp[i]) == true)
				targets.insert(std::pair<int, std::string>(0, tmp[i]));
			else
				err_noSuchNick();
		}
	}
	
	std::map<int, std::string>::const_iterator it;
	for (it = targets.begin(); it != targets.end(); it++)
	{
		switch (it->first) {
			case 0:
			{
				Reply	privmsg(*this, _nickname, _server->getClient(args[1]));
				if (_away == true)
					rpl_Away();
				privmsg.list("PRIVMSG");
				privmsg.list(args[1]);
				privmsg.list(args[2]);
				privmsg.ship();
				break;
			}
			case 1:
			{
				Reply	privmsg(*this, _nickname, *_server->getChannel(it->second, this), 'a', 'y');
				privmsg.list("PRIVMSG");
				privmsg.list(args[1]);
				privmsg.list(args[2]);
				privmsg.ship();
				break;
			}
			case 2:
			{
				Reply	privmsg(*this, _nickname, *_server->getChannel(it->second, this), 'o', 'y');
				privmsg.list("PRIVMSG");
				privmsg.list(args[1]);
				privmsg.list(args[2]);
				privmsg.ship();
				break;
			}
		}	
	}
	return;
}
