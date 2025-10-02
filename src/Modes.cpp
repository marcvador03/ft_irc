/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:03:03 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/02 14:46:29 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*MODE*/

static void	construct_modestring(t_arg &args, t_list &string)
{
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
	return;
}

static void	execute_modes(t_list &string, Channel *chan, Client &c)
{
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
					c.err_InvalidModeParam(*chan, "k", it->first, "use of invalid characters");
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
					c.err_InvalidModeParam(*chan, "l", it->first, "incorrect limit number");
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
	t_list	string;
	Channel	*chan;
	
	if (args.size() == 1)
		err_noSuchChannel(args[1]);
	else if (_server->isChannelExist(args[1]) == false)
		err_noSuchChannel(args[1]);
	else 
	{
		chan = _server->getChannel(args[1], *this);	
		if (args.size() == 2)
		{
			rpl_ChannelModeIs(*chan, args[1]);
			rpl_CreationTime(args[1]);
		}
		else if (args[2][0] != '+' && args[2][0] != '-')
			err_UModeUnknownFlag();
		else if (chan->isOperator(*this) == false)
			err_ChanOPrivsNeeded(args[1]);
		else
		{
			Reply	mode(*this, *chan, 'a', 'n');
			construct_modestring(args, string);
			execute_modes(string, chan, *this);
			mode.list(args[0]);
			mode.list(args[1]);
			mode.list(args[2]);
			mode.list(args[3]);
			mode.ship();
		}
	}
	return;
}
