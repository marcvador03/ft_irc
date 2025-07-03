/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/03 17:12:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server ( int timeout ): _timeout(timeout * 1000)
{
	std::cout << "Launching Server..." << std::endl;
	std::memset(&this->_server_addr, 0, sizeof(this->_server_addr));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(PORT);
	this->_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	this->_socklen = sizeof(this->_server_addr);
	std::cout << "Server launched" << std::endl;
	bind(this->_serverfd, (const struct sockaddr *)&(this->_server_addr), sizeof(this->_server_addr));
	std::cout << "Server bound to address and port: " << PORT << std::endl;
	listen(this->_serverfd, 5); //max number of connections to be determined
	std::cout << "Server is ready to listen" << std::endl;
	memset(this->fds, 0, sizeof(this->fds));
	this->fds[0].fd = this->_serverfd;
	this->fds[0].events = POLLIN;
	std::cout << "Server is listening" << std::endl;
}

Server::~Server( void )
{
	close (this->_serverfd);
	std::cout << "Server has been properly shutdown" << std::endl;
}

int	Server::getTimeOut( void ) const
{
	return this->_timeout;
}
