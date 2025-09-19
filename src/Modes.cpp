/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:03:03 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/19 19:17:45 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*MODE*/

static void	rpl_chanmode(Channel *chan, Reply &rpl_channelmodeis, std::string &target)
{
	rpl_channelmodeis.list(target);
	rpl_channelmodeis.list("Invite Only");
	rpl_channelmodeis.list(chan->isInviteOnly());
	rpl_channelmodeis.ship(324);
	rpl_channelmodeis.list("Test");
	rpl_channelmodeis.list("Topic Locked");
	rpl_channelmodeis.list(chan->isTopicLocked());
	rpl_channelmodeis.ship(324);
	rpl_channelmodeis.list(target);
	rpl_channelmodeis.list("Has Password");
	rpl_channelmodeis.list(chan->hasKey());
	rpl_channelmodeis.ship(324);
	rpl_channelmodeis.list(target);
	rpl_channelmodeis.list("Has Limit");
	rpl_channelmodeis.list(chan->getLimit());
	rpl_channelmodeis.ship(324);

	return;
}

static t_list	construct_modestring(t_arg &args)
{
	t_list	string;
	size_t	j = 3;
	
	for (size_t i = 1; i < args[2].size(); i++)
	{
		std::string	sign (1, args[2][i - 1]);
		for (; args[2][i] != '-' && args[2][i] != '+' && i < args[2].size(); i++)
		{
			if ((args[2][i] == 'l' || args[2][i] == 'k' || args[2][i] == 'o') && j < args.size())
				string.insert(std::pair<std::string, std::string>(sign + args[2][i], args[j++]));
			else if (args[2][i] == 'i' || args[2][i] == 't')
				string.insert(std::pair<std::string, std::string>(sign + args[2][i], ""));
		}
	}
	return string;
}

static void	execute_modes(t_list &string, Channel *chan, Client *c)
{
	Reply	err(*c, c->getNickname());
	std::map<std::string, int>	mod_table;
	
	mod_table["+i"] = 0;
	mod_table["-i"] = 1;
	mod_table["+k"] = 2;
	mod_table["-k"] = 3;
	mod_table["+o"] = 4;
	mod_table["-o"] = 5;
	mod_table["+t"] = 6;
	mod_table["-t"] = 7;
	mod_table["+l"] = 8;
	mod_table["-l"] = 9;
	
	t_list::const_iterator it;
	for (it = string.begin(); it != string.end(); it++)
	{
		switch(mod_table.find(it->first)->second) {
			case 0:
				chan->setInviteOnly(true);	
				break;
			case 1:
				chan->setInviteOnly(false);	
				break;
			case 2:
				if (chan->setKey(it->second) == -1)
				{
					err.list(chan->getName());
					err.list("k");
					err.list(it->second);
					err.list("use of invalid charachters");
					err.ship(696);
				}
				break;
			case 3:
				chan->unsetKey();
				break;
			case 4:
				chan->addOperator(c);
				break;
			case 5:
				chan->removeOperator(c);
				break;
			case 6:
				chan->setTopicLocked(true);
				break;
			case 7:
				chan->setTopicLocked(false);
				break;
			case 8:
			{
				size_t	l = 0;
				std::stringstream	ss(it->second);
				ss << it->second;
				ss >> l;
				if (l == 0)
				{
					err.list(chan->getName());
					err.list("l");
					err.list(it->second);
					err.list("incorrect limit number");
					err.ship(696);
					break;
				}
				chan->setLimit(l);
				break;
			}
			case 9:
				chan->unsetLimit();
				break;
		};
	}
	return ;
}
void Client::handleMode( t_arg args )
{
	Reply	mode(*this);
	Reply	err(*this, _nickname);
	t_list	string;
	Channel	*chan;
	
	if (args.size() == 1)
		err.ship(403);
	else if (_server->isChannelExist(args[1]) == false)
		err.ship(403);
	else 
	{
		chan = _server->getChannel(args[1], this);	
		if (args.size() == 2)
		{
			Reply	rpl_channelmodeis(*this, _nickname);
			rpl_chanmode(chan, rpl_channelmodeis, args[1]);
			Reply	rpl_creationtime(*this, _nickname);
			rpl_creationtime.list(args[1]);
			//rpl_creationtime.list(chan->getCreationTime());
			rpl_creationtime.ship(329);
		}
		else if (args[2][0] != '+' && args[2][0] != '-')
			err.ship(501);
		else if (chan->isOperator(this) == false)
			err.ship(482);
		else
		{
			construct_modestring(args);
			execute_modes(string, chan, this);
			mode.list(args[0]);
			mode.list(args[1]);
			mode.list(args[2]);
			mode.list(args[3]);
			mode.ship();
		}
	}
	return;
}
