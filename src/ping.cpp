/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 16:15:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/15 13:27:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Ping.hpp"

Ping::Ping ( t_arg args ): ACmd(args) {}
Ping::~Ping ( void ) {}
void Ping::run( Client &c ) 
{
	send(c.getClientfd(), "test", 4, 0);
}



