/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/04 16:51:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client (nfds_t serverfd, int slot): 
		_serverfd(serverfd),
		_slot(slot)
{
	this->_socklen = sizeof(this->_client_addr);
	this->_clientfd = accept(this->_serverfd, (struct sockaddr *)&this->_client_addr, &this->_socklen);
	std::cout << "Client connected" << std::endl;
}

Client::~Client( void )
{
	close (this->_clientfd);
	std::cout << "Client has been closed" << std::endl;
}

void	Client::ReceiveInput()
{
	char	buf[2048];
	int		bytes; 
	
	while (1)
	{
		std::memset(buf, 0, sizeof(buf));
		bytes = recv(this->_clientfd, buf, sizeof(buf), 0);
		if (bytes <= 0)
		{
			delete this;
			break;
		}
		else
		{
			buf[bytes] = '\0';
			this->_buffer_string = buf;		
			std::cout << "Receiving input: " << this->_buffer_string;
		}
	}
}

/*Getters and setters */
		
nfds_t		Client::getClientfd( void ) const
{
	return this->_clientfd;
}

int			Client::getSlot( void ) const
{
	return this->_slot;
}
