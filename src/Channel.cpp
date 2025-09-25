/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:17:51 by mpietrza          #+#    #+#             */
/*   Updated: 2025/09/23 17:47:37 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel ( const std::string name , Client *c):
	_name(name),
	_inviteOnly(false),
	_topicLocked(false),
	_hasKey(false),
	_hasLimit(false),
	_creationtime(std::time(NULL))
{
	std::cout << "Channel created" << std::endl;
	addMember(c);
	addOperator(c);
}

Channel::~Channel( void )
{
	std::cout << "Channel has been closed" << std::endl;
}

//membership management
std::map<int, Client *>	&Channel::getAllClients( void )
{
	return _clients;
}

void	Channel::addMember( Client *c )
{
	std::map<int, Client *>::iterator it = this->_clients.find(c->getSlot());
	if (it == this->_clients.end())
		this->_clients.insert(std::pair<int, Client *>(c->getSlot(), c));
}

void	Channel::removeMember( Client *c )
{
	std::map<int, Client *>::iterator it1 = this->_clients.find(c->getSlot());
	std::map<int, Client *>::iterator it2 = this->_operators.find(c->getSlot());
	if (it1 != this->_clients.end())
		this->_clients.erase(it1);
	if (it2 != this->_operators.end())
		this->_operators.erase(it2);
}

bool 	Channel::isMember( Client *c )
{
	std::map<int, Client *>::iterator it;
	it = _clients.find(c->getSlot());
	if (it == _clients.end())
		return false;
	return true;
}
//operator management
void Channel::addOperator(Client * c)
{
	std::map<int, Client *>::iterator it = this->_operators.find(c->getSlot());
	if (it == this->_operators.end())
		this->_operators.insert(std::pair<int, Client *>(c->getSlot(), c));
}

void Channel::removeOperator(Client * c)
{
	std::map<int, Client *>::iterator it = this->_operators.find(c->getSlot());
	if (it != this->_operators.end())
		this->_operators.erase(it);
}

bool Channel::isOperator(Client * c)
{
	std::map<int, Client *>::iterator it;
	it = _operators.find(c->getSlot());
	if (it == _operators.end())
		return false;
	return true;
}

//key (password) related
int Channel::setKey(const std::string &key)
{
	if (key.empty() == true)
		return -1;
	for (size_t i = 0; i < key.size(); i++)
	{
		if (key[i] == 32 || std::isalnum(key[i]) == false)
			return -1;
	}
	this->_hasKey = true;
	this->_key = key;
	return 0;
}

void Channel::unsetKey( void )
{
	this->_hasKey = false;
	this->_key = "";
	return ;
}

bool Channel::checkKey(const std::string &key) const
{
	
	if (_hasKey == true && key.compare(this->_key) == 0)
		return true;
	else if (_hasKey == false)
		return true;
	return  false;
}

bool Channel::hasKey(void) const
{
	return _hasKey;
}

//limit related
void 	Channel::unsetLimit( void )
{
	this->_hasLimit = false;
	this->_limit = 0;
}
void 	Channel::setLimit( size_t limit )
{
	this->_hasLimit = true;
	this->_limit = limit;
}

size_t	Channel::getLimit( void ) const
{
	return _limit;
}

bool 	Channel::hasReachedLimit( void ) const
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
	return _name;
}

std::string	Channel::getFullName( void ) const
{
	return this->_name;
}
	
std::string	Channel::getCreationTime( void ) const
{
	std::stringstream ss;

	ss << _creationtime;
	return ss.str();
}
//topic related
void 		Channel::setTopic( const std::string &topic )
{
	_topic = topic;
}

void 		Channel::setTopicLocked( bool b)
{
	_topicLocked = b;
}

std::string	Channel::getTopic( void ) const
{
	return this->_topic;
}

bool Channel::isTopicLocked() const
{
	return _topicLocked;
}

/*void Channel::broadcast_all(const std::string &msg, const std::string src)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Reply	broadcast(*it->second, src);
		if (it->second->getNickname() != src)
			broadcast.ship(msg);
	}
}*/

void Channel::broadcast_ops(const std::string &msg, const std::string src)
{
	for (std::map<int, Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		Reply	broadcast(*it->second, src);
		if (it->second->getNickname() != src)
			broadcast.ship(msg);
	}
}
