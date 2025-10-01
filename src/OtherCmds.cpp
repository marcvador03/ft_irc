/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OtherCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:43:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/01 16:53:35 by mfleury          ###   ########.fr       */
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
		err_NoOrigin();
		return ;
	}
	Reply	ping(*this);	
	ping.list("PONG");
	ping.list(args[1]);
	ping.ship();
	std::cout << "PONG sent with token: " << args[1] << std::endl;
	return;
}
