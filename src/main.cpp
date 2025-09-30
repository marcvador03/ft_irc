/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/30 16:49:39 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool	Server::signal = false;

void	handle_signal(int sig)
{
	(void) sig;
		
	std::cout << "Server interrupted by user Ctrl-C" << std::endl;
	Server::signal = true;
}

int	main ( int argc, char **argv )
{
	if (argc != 3) {
		std::cout << "Error! Wrong number of arguments!" << std::endl
				  << "Please use this order:  \"./ircserv <port> <password>\"" << std::endl;
		return (1);
	}
	char *endptr;
	int port = static_cast<int>strtol(argv[1]);
	if (endptr != argv[1]) ||  
	//<-- TO DO: check validity of port number and password format
	Server server(".irc42", "pass");
	signal(SIGINT, handle_signal);
	try {
		server.setSettings("irc_config");
		server.launch();
		server.listen_poll();
	}
	catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << std::endl;
	}
	return (0);
}








