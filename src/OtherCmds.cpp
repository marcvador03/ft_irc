/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OtherCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:43:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/08 15:20:23 by mpietrza         ###   ########.fr       */
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
