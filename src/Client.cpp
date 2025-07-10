/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/10 15:57:41 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client (nfds_t serverfd, int slot): 
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
	char	buf[2048];
	int		bytes;
	
	std::memset(buf, 0, sizeof(buf));
	bytes = recv(this->_clientfd, buf, sizeof(buf) - 1, 0);
	if (bytes == -1)
		throw Client::ErrnoException(); 
	else if (bytes == 0)
		delete this;
	else
	{
		buf[bytes] = '\0';
		std::cout << "Receiving input: " << buf;
		//mpietrza_2025-07-10
		std::isstringsream ss(buf);//allows treating strings as streams in c++ allowing to read data from sources
		std::string line;
		while (std::getline(ss, line)) {
			line = trim(line);
			if (line.empty())
				continue;

			std::string cmd_name = line.substr(0, line.find(' '));
			std::string args = line.substr(cmd_name.lenght());
			//Call handler based on cmd_name
			if (cmd_name == "NICK")
				handleNick(args);
			else if (cmd_name == "USER")
				handleUser(args);
			else if (cmd_name == "JOIN")
				handleJoin(args);
			//have to add the rest of commands here
			else
				reply("Unknown command: " +cmd_name);
		}
	}
}
/*
// Example command handler
void Client::handleNick(const std::string& args) {
    // Parse args and set nickname
}

void Client::handleUser(const std::string& args) {
    // Parse args and set username
}

void Client::handleJoin(const std::string& args) {
    // Parse args and join channel
}

void Client::reply(const std::string& msg) {
    send(this->_clientfd, msg.c_str(), msg.length(), 0);
}

// Helper function to trim whitespace
std::string Client::trim(const std::string& str) {
    const std::string WHITESPACE = " \n\r\t\f\v";
    size_t start = str.find_first_not_of(WHITESPACE);
    if (start == std::string::npos)
        return "";
    size_t end = str.find_last_not_of(WHITESPACE);
    return str.substr(start, end - start + 1);
}
*/
/*Getters and setters */
		
int		Client::getClientfd( void ) const
{
	return this->_clientfd;
}

int		Client::getSlot( void ) const
{
	return this->_slot;
}

/* helpers */
//trim
