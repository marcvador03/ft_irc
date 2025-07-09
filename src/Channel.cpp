/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:17:51 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/09 15:31:22 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel ( std::string name, std::string topic ):
	_name(name),
	_topic(topic)	
{
	std::cout << "Channel created" << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel has been closed" << std::endl;
}
void	Channel::addClient( int slot, Client *c )
{
	std::map<int, Client *>::iterator it = this->_clients.find(slot);
	if (it == this->_clients.end())
		this->_clients.insert(std::pair<int, Client *>(slot, c));
}

void	Channel::removeClient( int slot )
{
	std::map<int, Client *>::iterator it = this->_clients.find(slot);
	if (it != this->_clients.end())
		this->_clients.erase(it);
}

/*Getters and setters */
std::string	Channel::getName( void ) const
{
	return this->_name;
}

std::string	Channel::getTopic( void ) const
{
	return this->_topic;
}
