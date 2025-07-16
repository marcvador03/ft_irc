/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/16 15:13:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

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
			line = this->_trim(line);
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
	if (args[0] == "NICK")
		handleNick(*this);
}
/* Helpers */
//Helper for trimming white spaces
std::string	Client::_trim(const std::string &str) {
	const std::string WHITESPACE = " \n\r\t\f\v";
	size_t start = str.find_first_not_of(WHITESPACE);
	if (start == std::string::npos)
		//find_first_not_of() - C++ `std::string` class; this function is used to search a string (`str`) for the first character that does **not** match any of the characters specified in its argument
		// It returns the index of that character, or `std::string::npos` if all characters match the set
		return "";
	size_t end = str.find_last_not_of(WHITESPACE);
	return str.substr(start, end - start + 1);
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

