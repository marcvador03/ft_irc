/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACmd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:02:44 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/15 13:29:16 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ACmd.hpp"

ACmd::ACmd ( t_arg args ): _args(args) {}
ACmd::~ACmd ( void ) {}

void 	ACmd::run ( Client &c )
{
	(void)c;
	std::cout << "Received unknown command " << this->_args[0]; 
}
