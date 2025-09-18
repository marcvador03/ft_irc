/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OtherCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:43:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 13:10:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

/*PING*/
void Client::handlePing( t_arg args ) 
{
	Reply	ping(*this);	
	//check if the token is present and non-empty
	if (args.size() < 2 || args[1].empty()) 
	{
		ping.ship(409);
		return ;
	}
	ping.list("PONG");
	ping.list(args[1]);
	ping.ship();
	std::cout << "PONG sent with token: " << args[1] << std::endl;
	return;
}
