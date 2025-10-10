/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/10 15:59:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include <cerrno> // for errno

bool	Server::signal = false;

void	handle_signal(int sig)
{
	(void) sig;
		
	std::cout << "Server interrupted by user Ctrl-C" << std::endl;
	Server::signal = true;
}

static int checkPasswordChars(std::string str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] < '!' || str[i] > '~' || str[i] == '"' || str[i] == ',' || str[i] == ':' || str[i] == ';' || str[i] == '?' || str[i] == '*' || str[i] == '\\' || str[i] == '#' || str[i] == ' ' || str[i] == '\n' || str[i] == '\r' || str[i] == '\t')
			return i;
		i++;
	}
	return -1;
}

int	main ( int argc, char **argv )
{	
	if (argc != 3) {
		std::cout << "Error! Wrong number of arguments!" << std::endl
				  << "Please use this order:  \"./ircserv <port> <password>\"" << std::endl;
		return 1;
	}
	
	//port validation
	char *endptr;
	errno = 0;
	int portLong = strtol(argv[1], &endptr, 10);

	if (*endptr != '\0') {
		std::cout << "Error! Port must be a valid number!" << std::endl;
		return 1;
	}
	if (errno == ERANGE || portLong < 1 || portLong > 65535) {
		std::cout << "Error! Port number out of range (1 - 65535)!" << std::endl;
		return 1;
	}

	int port = static_cast<int>(portLong);

	//password validation
	std::string password = argv[2];
	
	if (password == "" || password.find(' ') != std::string::npos) {
		std::cout << "Error! Password cannot be empty or contain spaces!" << std::endl;
		return 1;
	}
	
	int wrongCharPos = checkPasswordChars(password);
	if (wrongCharPos != -1) {
		std::cout << "Error! Password cannot contain the character: \"" << password[wrongCharPos] << "\" on position " << wrongCharPos << std::endl;
		return 1;
	}

	//end: password validation

	Server server(".irc42", password);
	signal(SIGINT, handle_signal);
	try {
		server.setPort(port);
		//std::cout << "Server password set to: " << password << std::endl;
		server.setSettings("irc_config");
		server.launch();
		server.listen_poll();
	}
	catch (const std::exception& e) {
		std::cout << "Caught error: " << e.what() << std::endl;
	}
	return (0);
}








