/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserQry.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 09:30:03 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/21 19:21:39 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"

/*WHO*/
void	Client::handleWho( t_arg args )
{
	Channel								*chan = NULL;
	std::map<int, Client *> 			cli;
	std::map<int, Client *>::iterator	it;

	if (_server->isChannelExist(args[1]) == true)
	{
		chan = _server->getChannel(args[1]);
		cli = chan->getAllClients();
	}
	else if (_server->isClientExist(args[1]))
		cli.insert(std::make_pair<int, Client *>(0, &_server->getClient(args[1])));
	else
		return;
	for (it = cli.begin(); it != cli.end(); it++)
	{
		if (chan == NULL)
			rpl_WhoReply("*");
		else
			rpl_WhoReply(args[1]);	
	}
	rpl_EndOfWho(args[1]);
	return ;
}

/*WHOIS*/
void	Client::handleWhoIs( t_arg args )
{
	std::string nick;
	
	if (args.size() == 1)
		return(err_NoNicknameGiven());
	if (args.size() >= 3)
		nick = args[2];
	else
		nick = args[1];
	if (_server->isClientExist(nick) == false)
		return (err_noSuchNick(nick));
	rpl_WhoIsUser(nick);
	rpl_WhoIsServer(nick);
	rpl_WhoIsChannels(nick);
	//rpl_WhoIsActually(nick);
	rpl_WhoIsHost(nick);
	rpl_Away(nick);
	rpl_EndofWhoIs(nick);
	return ;
}

/*AWAY*/
void	Client::handleAway( t_arg args )
{
	std::string msg;
	if (args.size() >= 2)
		msg = args[1];

	//truncate a AWAY message to AWAYLEN value from irc_config
	int maxLen = _server->getLen("AWAYLEN", "Away", 200);
	if (msg.size() > static_cast<size_t>(maxLen))
		msg.resize(maxLen);
	
	if (msg.empty())
	{
		_away = false;
		_awaymsg.clear();
		rpl_Unaway();
	}
	else
	{
		_away = true;
		_awaymsg = msg;
		rpl_NowAway();
	}
	return;
}
