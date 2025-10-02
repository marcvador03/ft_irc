/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/10/02 13:38:28 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utils.hpp"
#include "../inc/Client.hpp"

Client::Client (Server *s, int slot): //we will need to revisit all Server parameters set at start!!//
		_server(s),
		_slot(slot),
		_isPassAccepted(false),
		_isRegistered(false),
		_hasNick(false),
		_hasUser(false),
		_chanlim(10),
		_away(false)
{
	this->_socklen = sizeof(this->_client_addr);
	this->_clientfd = accept(_server->getFd(), (struct sockaddr *)&this->_client_addr, &this->_socklen);
	if (this->_clientfd == -1)
		throw Client::ErrnoException();
	std::cout << "Client connected" << std::endl;
}

Client::~Client( void )
{
	close (this->_clientfd);
	std::cout << "Client has been closed" << std::endl;
}

/* Getters and setters */
int		Client::getClientfd( void ) const
{
	return this->_clientfd;
}

int		Client::getSlot( void ) const
{
	return this->_slot;
}

std::string	Client::getServername( void ) const
{
	return (_server->getName());
}

std::string	Client::getServerLaunchTime( void ) const
{
	return (_server->getLaunchTime());
}

std::string	Client::getServerVersion( void ) const
{
	return (_server->getVersion());
}

t_settings	Client::getServerSettings( void ) const
{
	return (_server->getSettings());
}

std::string	Client::getServerSetting( const std::string str ) const
{
	return (_server->getSetting(str));
}

std::map<int, Client *> Client::getServerClientList( void ) const
{
	return (_server->getAllClients());
}

std::string	Client::getNickname( void ) const
{
	return this->_nickname;
}

int	Client::setNickname( std::string &nick)
{
	//check if nickname is provided
	if (nick.empty())
		return 431;
	//validate nickname format
	if (nick[0] == '#' || nick[0] == ':' || std::isspace(nick[0]))
		return 432;
	for (size_t i = 0; i < nick.size(); ++i)
	{
		char c = nick[i];
		if (!std::isalnum(c) && std::string("[]{}\\|").find(c) == std::string::npos)
			return 432;
	}
	if (this->_server->InsertNick(nick) == false)
		return 433;
	this->_nickname = nick;
	_hasNick = true;
	return _completeReg();
}

int	Client::setUser( std::string &user, std::string &real)
{
	if (user.empty() == true|| real.empty() == true)
		return 431;
	//missing validating user length
	this->_username = user;
	this->_realname = real;
	_hasUser = true;
	return _completeReg();
}

std::string	Client::getHost( void ) const
{
	return this->_host;
}

std::string	Client::getUser( void ) const
{
	return this->_username;
}

int		Client::leaveChannel( std::string name)
{
	Channel *ch;
	
	if (_server->isChannelExist(name) == false)
		return 403;
	else if (isPartofChannel(name) == false)
		return 442;
	ch = _server->getChannel(name, *this);
	ch->removeMember(*this);
	_channels.erase(ch);
	return 0;
}

int		Client::leaveAllChannels( void )
{
	std::set<Channel *>::iterator it;
	Channel *ch;
	std::string name;
	
	for (it = this->_channels.begin(); it != this->_channels.end() ; it++)
	{
		name = (*it)->getName();
		ch = _server->getChannel(name, *this);
		ch->removeMember(*this);
		_channels.erase(ch);
		if (_channels.empty() == true)
			return (0);
	}
	return (0);
}
	
int		Client::joinChannel( std::string name, std::string key )
{
	Channel *ch;
	
	if (_channels.size() >= _chanlim)
		return 405;
	if (name[0] != '&' && name[0] != '#')
		return 476;
	if (	name.find(' ') != std::string::npos \
			|| name.find(0x07) != std::string::npos \
			|| name.find(',') != std::string::npos)
		return 476;
	ch = _server->getChannel(name, *this);
	if (ch->checkKey(key) == false)
		return 475;
	if (ch->isInviteOnly() == true)
		return 473;
	if (ch->hasReachedLimit() == true)
		return 471;
	ch->addMember(*this);
	_channels.insert(ch);
	return 0;
}

bool	Client::isPartofChannel( std::string &name ) 
{
	Channel *ch;
	
	ch = _server->getChannel(name, *this);
	return (ch->isMember(*this));
}
		
int		Client::registerPass( std::string &pass)
{
	if (pass.empty() == true && _server->checkPass(pass) == false)
		return 461;
	if (this->_isRegistered == true)
		return 462;
	if (_server->checkPass(pass) == false)
		return 464;
	_isPassAccepted = true;
	return 0;
}

int		Client::_completeReg( void )
{
	if (_isRegistered == false && _isPassAccepted == true && _hasNick == true && _hasUser == true)
	{
		_isRegistered = true;
		return 1;
	}
	return 0;
}
bool		Client::isPasswordAccepted( void ) const
{
	return (_isPassAccepted);
}

bool		Client::isRegistered( void ) const
{
	return (_isRegistered);

}
