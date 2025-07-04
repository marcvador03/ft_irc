/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/04 16:04:31 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

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
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		this->_slots[i] = false;
	this->pfd[0].fd = this->_serverfd;
	this->pfd[0].events = POLLIN;
	this->_slots[0] = true;
	std::cout << "Server is listening" << std::endl;
}

Server::~Server( void )
{
	close (this->_serverfd);
	std::cout << "Server has been properly shutdown" << std::endl;
}

void	Server::addClient( void )
{
	Client	*c;
	int	slot = this->getFirstSlot();
	c = new Client(this->_serverfd, slot);
	this->pfd[slot].fd = c->getClientfd();
	this->pfd[slot].events = POLLIN;
	this->connections.insert(std::make_pair(slot, c));
	std::cout << "A new Client was added" << std::endl;
}

void	Server::removeClient( const Client *client )
{
	std::map<int, Client *>::iterator it;
	it = connections.find(client->getSlot());
	if (it != connections.end())
	{
		delete it->second;
		connections.erase(it);
		std::cout << "Client was disconnected" << std::endl;
	}
}

/* Setters, Getters and private functions to manage available slot list */

int	Server::getTimeOut( void ) const
{
	return this->_timeout;
}
int	Server::getFirstSlot( void )
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (this->_slots[i] == false)
		{
			this->_slots[i] = true;
			return i;
		}
	}
	//if max connections is reached?
	return (-1);
}

void	Server::setFreeSlot( const int i )
{
	this->_slots[i] = false;
	//if i > max connections?
}

void	Server::setBusySlot( const int i)
{
	this->_slots[i] = true;
	//if i > max connections?
}
