/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 14:34:27 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.h"

bool	Server::signal = false;

void	handle_signal(int sig)
{
	(void) sig;
		
	std::cout << "Server interrupted by user Ctrl-C" << std::endl;
	Server::signal = true;
}

int	main ( void )
{
	Server server;
	signal(SIGINT, handle_signal);
	try {
		server.launch();
		server.listen_poll();
	}
	catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << std::endl;
	}
	return (0);
}








