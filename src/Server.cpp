/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/12 17:42:11 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/Server.hpp"

Server::Server ( void )
{
	std::cout << "Launching Server..." << std::endl;
	std::memset(&this->server_addr, 0, sizeof(this->server_addr));
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_addr.s_addr = INADDR_ANY;
	this->server_addr.sin_port = htons(PORT);
	this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
	this->socklen = sizeof(this->server_addr);
	std::cout << "Server launched" << std::endl;
	bind(this->serverfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	std::cout << "Server bound to address and port: " << PORT << std::endl;
	listen(this->serverfd, 5);
	std::cout << "Server is listening" << std::endl;
}
Server::~Server( void )
{
	close (this->serverfd);
}
Server::Server ( const Server &s )
{
}
Server& Server::operator=( const Server &s )
{
}
