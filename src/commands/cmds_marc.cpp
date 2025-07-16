/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_marc.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:49:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 09:53:02 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/commands/cmds_marc.hpp"

void handlePing( Client &c ) 
{
	send(c.getClientfd(), "test", 4, 0);
	std::cout << "ping sent" << std::endl;
}

/*void handleNick( Client &c ) 
{
	this->_nickname = this->_args[1];
	std::cout << "nickname updated" << std::endl;
}*/


