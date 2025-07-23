/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:17:51 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/23 15:45:16 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel ( std::string name ):
	_name(name),
	_inviteOnly(false),
	_topicLocked(false),
	_hasKey(false),
	_hasLimit(false)
{
	std::cout << "Channel created" << std::endl;
}

Channel::~Channel( void )
{
	std::cout << "Channel has been closed" << std::endl;
}

//membership management
void	Channel::addMember( Client *c )
{
	std::map<int, Client *>::iterator it = this->_clients.find(c->getSlot());
	if (it == this->_clients.end())
		this->_clients.insert(std::pair<int, Client *>(c->getSlot(), c));
}

void	Channel::removeMember( Client *c )
{
	std::map<int, Client *>::iterator it = this->_clients.find(c->getSlot());
	if (it != this->_clients.end())
		this->_clients.erase(it);
}

bool 	Channel::isMember( Client *c )
{
	std::map<int, Client *>::iterator it;
	it = _clients.find(c->getSlot());
	if (it == _clients.end())
		return false;
	return true;
}

//key (password) related
void Channel::setKey(const std::string &key)
{
	this->_hasKey = true;
	this->_key = key;

}

bool Channel::checkKey(const std::string &key) const
{
	
	if (_hasKey == true && key.compare(this->_key) == 0)
		return true;
	else if (_hasKey == false)
		return true;
	return  false;
}

//limit related
void Channel::setLimit(unsigned int limit)
{
	this->_hasLimit = true;
	this->_limit = limit;
}

bool Channel::hasReachedLimit() const
{
	if (this->_hasLimit == true && this->_clients.size() >= this->_limit)
		return true;
	return false;
}

//invite only related
void Channel::setInviteOnly(bool isInviteOnly)
{
	this->_inviteOnly = isInviteOnly;
}

bool Channel::isInviteOnly() const
{
	return (this->_inviteOnly);
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

void Channel::broadcast(const std::string &message, const Client *exclude)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second != exclude)
			it->second->reply(message);
	}
}

/* membership management*/
bool Channel::isMember(Client *client) const
{
    for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second == client)
            return true;
    }
    return false;
}

bool Channel::removeMember(Client *client)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second == client)
        {
            it->second->_channels.erase(this); // Remove channel from client's set if you track it
            _clients.erase(it);
            return true;
        }
    }
    return false;
}
