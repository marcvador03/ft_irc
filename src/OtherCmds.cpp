/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OtherCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:43:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/10 12:25:02 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

/*PING*/
void Client::handlePing( t_arg args ) 
{
	//check if the token is present and non-empty
	if (args.size() < 2 || args[1].empty()) 
	{
		err_NoOrigin(); //409
		return ;
	}
	std::string token = args[1];
	if (!token.empty() && token[0] == ':')
		token.erase(0, 1);

	Reply reply(*this);	
	reply.list("PONG");
	reply.list(getServername());
	reply.list(token);
	reply.ship();

	std::cout << "PONG (token=" << token << ")" << std::endl;
	return;
}

void Client::handleQuit( t_arg args )
{
	std::string reason;
	if (args.size() > 1)
		reason = args[1];
	if (!reason.empty() && reason[0] == ':')
		reason.erase(0,1);
	if (reason.empty())
		reason = "Client Quit";
	std::string src = getNickname();
	if (!getUser().empty())
		src += "!" + getUser();
	if (!getHost().empty())
		src += "@" + getHost();
	for (std::set<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Reply r(*this, src, **it, 'a', 'y');
		r.list("QUIT");
		r.list(reason);
		r.ship();
	}
	Reply self(*this, src, *this); // send back using same prefix
	self.list("ERROR");
	self.list("Closing Link: " + reason);
	self.ship();
	markForDisconnect();
}
