/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/30 17:08:04 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include <cerrno> // for errno
#include <climits> // for INT_MIN and MAX

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
		return 1;
	}

	char *endptr;
	errno = 0;
	int portLong = strtol(argv[1]);

	if (endptr != '\0') {
		std::cout << "Error! Port must be a valid number!" << std::endl;
		return 1;
	}
	if (errn == ERANGE || portLong < 1 || portLong > 65535) {
		std::cout << "Error! Port number out of range (1 - 65535)!" << std::endl;
		return 1;
	}

	int port = static_cast<int>(port_long);

	//<-- TO DO: exporting the port number to use in other files

	//<-- TO DO: password validation
	
	char 

	//end: password validation

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








