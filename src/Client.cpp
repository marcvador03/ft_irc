/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/23 14:18:16 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"

Client::Client (Server *s, int slot): //we will need to revisit all Server parameters set at start!!//
		_server(s),
		_slot(slot),
		_isPassAccepted(false),
		_isRegistered(false),
		_hasNick(false),
		_hasUser(false),
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
		for (std::string line; std::getline(ss, line);) 
		{
			line = trim(line);
			if (line.empty())
				continue;
			std::istringstream sub_line(line);
			sub_line >> std::ws; //to remove leading WS
			for (int i = 0; sub_line.peek() != EOF; i++)
			{
				if ( i > 0 && sub_line.peek() == ':')
				{
					sub_line.seekg(1, std::ios::cur);
					std::getline(sub_line, args[i]);
				}
				else
					std::getline(sub_line, args[i], ' ');
			}
			this->LaunchCmd();
			args.erase(args.begin(), args.end());
		}
	}
}

void	Client::LaunchCmd()
{
	if (_isPassAccepted == false)
	{
		if (args[0] == "PASS")
			handlePass(*this);
		else if (args[0] == "PING")
			handlePing(*this);
		return ;
	}
	if (_isPassAccepted == true && _isRegistered == false)
	{
		if (args[0] == "NICK")
			handleNick(*this);
		else if (args[0] == "USER")
			handleUser(*this);
		else if (args[0] == "PING")
			handlePing(*this);
		return ;
	}
	if (args[0] == "PING")
		handlePing(*this);
	else if (args[0] == "NICK")
		handleNick(*this);
	else if (args[0] == "JOIN")
		handleJoin(*this);
	else if (args[0] == "USER")
		handleUser(*this);
}

/* Methods to send back replies to Client, source default servername */
void	Client::_send(std::string &str)
{
	ssize_t		bytes;
	
	bytes = send(this->_clientfd, str.c_str(), str.length(), 0);
	std::cout << bytes << " bytes have been sent to " << this->_nickname << std::endl;
}

void 	Client::reply(const std::string &msg) 
{
	this->reply(".servername", msg);
}

void 	Client::reply(t_cmd_reply &cmd) 
{
	this->reply(".servername", cmd);
}

void 	Client::reply(const int num) 
{
	this->reply(".servername", num);
}

void 	Client::reply(const int num, t_cmd_reply &cmd) 
{
	this->reply(".servername", num, cmd);
}

/* Methods to send back replies to Client, override source*/
void 	Client::reply(const std::string &src, const std::string &msg) 
{
	std::string	str;

	str = ":" + src + " ";
	if (msg[msg.size() - 1] != '\n')
	{
		if (msg[msg.size() - 2] != '\r')
			str = msg + "\r\n";
		else
			str = msg + "\n";
	}
	else
		str += msg;
	_send(str);
}

void 	Client::reply(const std::string &src, t_cmd_reply &cmd) 
{
	std::string	str;
	t_cmd_reply::iterator it;

	str = ":" + src + " ";
	//str = ":" + this->_server->getName() + " ";
	it = cmd.begin();
	for (it = cmd.begin(); it != cmd.end(); it++)
	{
		if ((it + 1) == cmd.end())
			str += ":" + *it + " ";
		else
			str += *it + " ";
	}
	str += "\n";
	_send(str);
}

void 	Client::reply(const std::string &src, const int num) 
{
	std::string	str;
	std::stringstream ss;
	int	i;

	str = ":" + src + " ";
	ss << num;
	for (i = 0; ss.get() != EOF; i++);
	if (i == 0 || i > 3)
	{
		std::cout << "Error in reply command, too many numeric" << std::endl;
		return;
	}
	else if (i == 1)
	{
		ss.putback('0');
		ss.putback('0');
	}
	else if (i == 2)
		ss.putback('0');
	ss.seekg(0);
	str += ss.str() + " ";
	str += "\r\n";
	_send(str);
}

void 	Client::reply(const std::string &src, const int num, t_cmd_reply &cmd) 
{
	std::string	str;
	std::stringstream ss;
	t_cmd_reply::iterator it;

	str = ":" + src + " ";
	ss << num;
	if (num < 0 || num > 999)
	{
		std::cout << "Error in reply command, too many numeric" << std::endl;
		return;
	}
	else if (num < 10)
		str += "00" + ss.str() + " ";
	else if (num < 100)
		str += "0" + ss.str() + " ";
	else
		str += ss.str() + " ";
	it = cmd.begin();
	for (it = cmd.begin(); it != cmd.end(); it++)
	{	
		if ((it + 1) == cmd.end())
			str += ":" + *it + " ";
		else
			str += *it + " ";
	}
	str += "\r\n";
	_send(str);
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
