/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_marc.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:49:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 15:10:03 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/Client.hpp"

void handlePing( Client &c ) 
{
	send(c.getClientfd(), "test", 4, 0);
	std::cout << "ping sent" << std::endl;
}

void handleNick( Client &c ) 
{
	c.setNickname (c.args[1]);
	std::cout << "nickname updated" << std::endl;
}


