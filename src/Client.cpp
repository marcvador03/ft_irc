/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/03 21:26:31 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client (nfds_t serverfd ): _serverfd(serverfd)
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

std::string	Client::ReceiveInput()
{
	std::cout << "Receiving input" << std::endl;
	return "test";
}
