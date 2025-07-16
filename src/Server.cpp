/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 13:52:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server ( void )
{
}

Server::~Server( void )
{
	this->closefds();
}

void	Server::launch( void )
{
	this->_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverfd == -1)
		throw Error::ErrnoException(); 
	std::cout << "Launching Server..." << std::endl;
	std::memset(&this->_server_addr, 0, sizeof(this->_server_addr));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(PORT);
	this->_socklen = sizeof(this->_server_addr);
	std::cout << "Server launched" << std::endl;
	if (bind(this->_serverfd, (const struct sockaddr *)&(this->_server_addr), sizeof(this->_server_addr)) == -1)
		throw Error::ErrnoException(); 
	std::cout << "Server bound to address and port: " << PORT << std::endl;
}

void	Server::listen_poll( void )
{
	if (listen(this->_serverfd, MAX_CONNECTIONS) == -1)
		throw Error::ErrnoException(); 
	std::cout << "Server is ready to listen" << std::endl;
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		this->_slots[i] = false;
	this->_pfd[0].fd = this->_serverfd;
	this->_pfd[0].events = POLLIN;
	this->_slots[0] = true;
	std::cout << "Server is listening and starts polling" << std::endl;
	while (Server::signal == false)
	{
		if (poll(this->_pfd, 200, -1) == -1)
			throw Error::ErrnoException(); 
		for (long unsigned int i = 0; i <= this->_connections.size(); i++) // monitors all connected clients sockets for their commands
		{
			if (this->_pfd[i].revents & POLLIN)
			{
				if (i == 0)
					this->addClient();
				else
					this->_connections[i]->ReceiveInput();
			}
		}
	}
	this->closefds();
}

void	Server::closefds( void )
{
	close(this->_serverfd);
	std::cout << "Server has been properly shutdown" << std::endl;
}

void	Server::addClient( void )
{
	Client	*c;

	int	slot = this->getFirstSlot();
	c = new Client(this->_serverfd, slot);
	this->_pfd[slot].fd = c->getClientfd();
	this->_pfd[slot].events = POLLIN;
	this->_connections.insert(std::make_pair(slot, c));
	std::cout << "A new Client was added" << std::endl;
}

void	Server::removeClient( const Client *client )
{
	std::map<int, Client *>::iterator it;
	it = _connections.find(client->getSlot());
	if (it != _connections.end())
	{
		delete it->second;
		_connections.erase(it);
		std::cout << "Client was disconnected" << std::endl;
	}
}

/* Setters, Getters and private functions to manage available slot list */

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
