/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/17 16:34:10 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"


Client::Client (int serverfd, int slot): 
		_serverfd(serverfd),
		_slot(slot)
{
	this->_socklen = sizeof(this->_client_addr);
	this->_clientfd = accept(this->_serverfd, (struct sockaddr *)&this->_client_addr, &this->_socklen);
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
	std::string line;
	int			bytes;
	int			i;
	std::string::size_type start_pos;
	
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
		start_pos = 0;
		i = 0;
		while (std::getline(ss, line)) 
		{
			line = trim(line);
			if (line.empty())
				continue;
			args[i++] = line.substr(start_pos, line.find(' '));
			start_pos = line.find(' ');
			if (start_pos == std::string::npos)
				break;
		}
		this->LaunchCmd();
	}
}

void	Client::LaunchCmd()
{
	if (args[0] == "PING")
		handlePing(*this);
	else if (args[0] == "NICK")
		handleNick(*this);
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

void		Client::setNickname( std::string &nick)
{
	if (nick.empty() == true)
		std::cout << this->_clientfd << ": no nickname provided" << std::endl;
	this->_nickname = nick;
}

std::string	Client::getName( void ) const
{
	return this->_name;
}

void		Client::setName( std::string &name)
{
	this->_name = name;
}
void Client::reply(const std::string& msg) 
{
	send(this->_clientfd, msg.c_str(), msg.length(), 0);
}
