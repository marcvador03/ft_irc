/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/22 19:16:07 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"

Client::Client (Server *s, int slot): //we will need to revisit all Server parameters set at start!!//
		_server(s),
		_slot(slot),
		_chanlim(10)
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

void	Client::ReceiveInput()
{
	char		buf[2048];
	int			bytes;
	
	std::memset(buf, 0, sizeof(buf));
	bytes = recv(this->_clientfd, buf, sizeof(buf) - 1, 0);
	if (bytes == -1)
		throw Client::ErrnoException(); 
	else if (bytes == 0)
	{
		delete this;
		return;
	}
	else
	{
		buf[bytes] = '\0';
		std::cout << "Receiving input: " << buf;
		std::istringstream ss(buf);
		//ss >> std::ws; //to remove leading WS
		for (std::string line; std::getline(ss, line);) 
		{
			line = trim(line);
			if (line.empty())
				continue;
			std::istringstream sub_line(line);
			sub_line >> std::ws; //to remove leading WS
			for (int i = 0; std::getline(sub_line, args[i], ' '); i++);
			this->LaunchCmd();
			args.erase(args.begin(), args.end());
		}
	}
}

void	Client::LaunchCmd()
{
	if (args[0] == "PING")
		handlePing(*this);
	else if (args[0] == "NICK")
		handleNick(*this);
	else if (args[0] == "JOIN")
		handleJoin(*this);
}

void 	Client::reply(const std::string& msg) 
{
	ssize_t		bytes;
	std::string	str;

	if (msg[msg.size() - 1] != '\n')
		str = msg + "\n";
	bytes = send(this->_clientfd, str.c_str(), str.length(), 0);
	std::cout << bytes << " bytes have been sent to " << this->_nickname << std::endl;
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
	return 0;
}

std::string	Client::getName( void ) const
{
	return this->_name;
}

void	Client::setName( std::string &name)
{
	this->_name = name;
}

int		Client::leaveChannel( std::string name)
{
	Channel *ch;
	
	if (_server->isChannelExist(name) == false)
		return 403;
	else if (isPartofChannel(name) == false)
		return 442;
	ch = _server->getChannel(name);
	ch->removeMember(this);
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
		ch = _server->getChannel(name);
		ch->removeMember(this);
		_channels.erase(ch);
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
	ch = _server->getChannel(name);
	if (ch->checkKey(key) == false)
		return 475;
	if (ch->isInviteOnly() == true)
		return 473;
	if (ch->hasReachedLimit() == true)
		return 471;
	ch->addMember(this);
	_channels.insert(ch);
	return 0;
}

bool	Client::isPartofChannel( std::string &name ) 
{
	Channel *ch;
	
	ch = _server->getChannel(name);
	return (ch->isMember(this));
}
