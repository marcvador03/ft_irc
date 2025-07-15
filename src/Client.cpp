/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:50:46 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/15 14:32:36 by mfleury          ###   ########.fr       */
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
		delete this;
	else
	{
		buf[bytes] = '\0';
		std::cout << "Receiving input: " << buf;
		//mpietrza_2025-07-10
		std::istringstream ss(buf);
		//allows treating strings as streams in c++ allowing to read data from sources
		start_pos = 0;
		i = 0;
		while (std::getline(ss, line)) 
		{
			line = this->_trim(line);
			if (line.empty())
				continue;
			_args[i] = line.substr(start_pos, line.find(' '));
			start_pos = line.find(' ');
			if (start_pos == std::string::npos)
				break;
/*			std::string cmd_name = line.substr(0, line.find(' '));
			std::string args = line.substr(cmd_name.lenght());*/
/*			//Call handler based on cmd_name
			if (cmd_name == "NICK")
				handleNick(args);
			else if (cmd_name == "USER")
				handleUser(args);
			else if (cmd_name == "JOIN")
				handleJoin(args);
			//have to add the rest of commands here
			else
				reply("Unknown command: " + cmd_name);*/
		}

		(this->*_memberPtr)(this->_args);

	}
}

void	Client::launch_cmd( void )
{
}
/* Commands handling */

/** Command: JOIN
 * @brief The JOIN command is used to join a channel. 
 * If the channel does not exist, it will be created.
 * <key> is an optional parameter that can be used to join a channel with a password.
 * @param <channel>{,<channel>} [<key>]
 * @return Void;
 */
/*void Client::handleJoin(const std::string &args) {
    // Parse args and join channel
}*/

/** Command: PART
 * @brief The PART command removes the client from the given channel(s). 
 * On sending a successful PART command, the user will receive a PART 
 * message from the server for each channel they have been removed from.
 * <reason> is the reason that the client has left the channel(s).
 * @param <channel>{,<channel>} [<reason>]
 * @return Void;
 */
/*void Client::handlePart(const std::string &args) {
	// Parse args and part from channel
}*/

/** Command: TOPIC
 * @brief The TOPIC command is used to change or view the topic 
 * of the given channel. If <topic> is not given, either RPL_TOPIC 
 * or RPL_NOTOPIC is returned specifying the current channel topic 
 * or lack of one. If <topic> is an empty string, the topic
 * for the channel will be cleared.
 * @param <channel> [<topic>]
 * @return Void;
 */
/*void Client::handleTopic(const std::string &args) {
	// Parse args and set topic for channel
}*/

/** Command: INVITE
 * @brief The INVITE command is used to invite a user to a channel.
 * The parameter <nickname> is the nickname of the person to be 
 * invited to the target channel <channel>.
 * @param <nickname> <channel>
 * @return Void;
 */
/*void Client::handleInvite(const std::string &args) {
	// Parse args and invite user to channel
}*/


/*
...
 */

/*void Client::reply(const std::string& msg) {
    send(this->_clientfd, msg.c_str(), msg.length(), 0);
}*/

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

/* helpers */
//trim
