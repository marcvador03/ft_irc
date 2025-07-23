/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/23 10:43:27 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/utils.hpp"


Client::Client (Server *s, int slot): 
		_server(s),
		_slot(slot)
{
	this->_socklen = sizeof(this->_client_addr);
	this->_clientfd = accept(_server->get_Fd(), (struct sockaddr *)&this->_client_addr, &this->_socklen);
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
			for (int i = 0; sub_line.peek() != EOF; i++)
			{
				if ( i > 0 && sub_line.peek() == ':')
					std::getline(sub_line, args[i]);
				else
					std::getline(sub_line, args[i], ' ');
			}
			//for (int i = 0; std::getline(sub_line, args[i], ' '); i++);
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
	if (this->_server->insert_nickname(nick) == false)
		return 433;
	this->_nickname = nick;
	return 0;
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
	ssize_t	bytes;

	bytes = send(this->_clientfd, msg.c_str(), msg.length(), 0);
	std::cout << bytes << " bytes have been sent to " << this->_nickname << std::endl;
}
