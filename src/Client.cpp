/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/10/14 20:26:56 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utils.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

Client::Client (Server *s, int slot): //we will need to revisit all Server parameters set at start!!//
		_server(s),
		_slot(slot),
		_isPassAccepted(false),
		_isRegistered(false),
		_hasNick(false),
		_hasUser(false),
		_chanlim(s->getChanLim()),
		_away(false),
		_awaymsg("")
{
	struct sockaddr_in 	addr;
	char 				ip_addr[INET_ADDRSTRLEN];
	socklen_t			addr_len;

	addr_len = sizeof(addr);
	std::memset(&addr, 0, sizeof(addr));
	this->_socklen = sizeof(this->_client_addr);
	this->_clientfd = accept(_server->getFd(), (struct sockaddr *)&this->_client_addr, &this->_socklen);
	if (this->_clientfd == -1)
		throw Client::ErrnoException();
	if(getsockname(_clientfd, (struct sockaddr *)&addr, &addr_len) == -1)
		throw Client::ErrnoException();
	_host = inet_ntop(AF_INET, &(addr.sin_addr), ip_addr, INET_ADDRSTRLEN);
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

static bool isNickValidSymbol( unsigned char c )
{
	return ( c == '[' || c == ']' || c =='\\' || c == '`' || c == '_' || c == '^' || c == '{' || c == '}' || c == '|' );
}


bool Client::isNicknameValid( std::string &nick )
{
	if (nick.size() > static_cast<unsigned long>(_server->getNickLen()))
		return false;

	unsigned char c0 = static_cast<unsigned char>(nick[0]);

	if (!(isNickValidSymbol(c0) || std::isalpha(c0))) 
		return false;
	for (size_t i = 1; i < nick.size(); ++i) 
	{
		unsigned char c = static_cast<unsigned char>(nick[i]);
		if (!(isNickValidSymbol(c) || std::isalnum(c) || c == '-'))
			return false;
	}
	return true;		
}

int	Client::setNickname( std::string &nick)
{
	//check if nickname is provided
	if (nick.empty())
		return 431;
	//validate nickname format
	if (Client::isNicknameValid(nick) == false)
		return 432;
	//check if nickname is already in use
	if (this->_server->InsertNick(nick) == false)
		return 433;
	this->_nickname = nick;
	_hasNick = true;
	return _completeReg();
}

bool Client::isUsernameValid( std::string &user )
{
	if (user.size() > static_cast<unsigned long>(_server->getUserLen()))
		return false;

	for (size_t i = 0; i < user.size(); ++i) 
	{
		unsigned char c = static_cast<unsigned char>(user[i]);
		if (c == 0 || c == '\r' || c == '\n' || c == ' ' || c == '@')
			return false;
		if (!std::isprint(c))
			return false;
	}
	return true;		
}

int	Client::setUser( std::string &user, std::string &real)
{
	//check if username and real name are provided
	if (user.empty() == true || real.empty() == true)
		return 461;
	//validate username format
	if (Client::isUsernameValid(user) == false)
		return 461;
	//validagte realname format <------------------------------------------------- TO DO
	//if (isValidRealname(real) == false) //TO DO
	//	return 461;

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

std::string	Client::getRealname( void ) const
{
	return this->_realname;
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
	if (ch->isInviteOnly() == true && ch->isInvited(*this) == false)
		return 473;
	if (ch->hasReachedLimit() == true)
		return 471;
	ch->addMember(*this);
	ch->removeInvite(*this);
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
	if (pass == "")
		return 1; //no password set on server;
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
	
bool		Client::getAwayStatus( void ) const
{
	return _away;
}

std::string	Client::getAwayMsg( void ) const
{
	return _awaymsg;
}
