/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/06 15:35:03 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.h"

int	main ( void )
{
	Server server;
	try {
		server.launch();
		server.listen_poll();
	}
	catch (const std::exception& e) {
		std::cout << "Caugh error: " << e.what() << std::endl;
	}
	return (0);
}
