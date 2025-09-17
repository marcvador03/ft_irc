/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OtherCmds.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:43:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/17 13:43:34 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

/*PING*/
void Client::handlePing( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	
	//check if the token is present and non-empty
	if (args.size() < 2 || args[1].empty()) 
	{
		reply(409);
		return ;
	}
	cmd_reply.push_back("PONG");
	cmd_reply.push_back(args[1]);
	reply(cmd_reply);
	std::cout << "PONG sent with token: " << args[1] << std::endl;
	return;
}
