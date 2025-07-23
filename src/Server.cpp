/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/24 00:13:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server ( const std::string &servername, const std::string &pass ): // we will need to update default inputs with program argv!!
	_name(servername),
	_password(pass),
	_version("1.0")	
{
	std::time_t now = std::time(NULL);
	std::strftime(_launchtime, sizeof(_launchtime), "%A %c", std::localtime(&now));

	std::ifstream	config;
	std::vector<std::string> tmp;
	std::string 	line;

	config.open("irc_config");
	if (config.is_open() == true)
	{
		for (int i = 0, j = 0; std::getline(config, line) ; i++, j++)
		{
			tmp.push_back(line);
			if (j == 13)
			{	
				_settings.push_back(tmp);
				j = 0;
				tmp.clear();
			}
		}
		if (tmp.size() > 0)
			_settings.push_back(tmp);
		tmp.clear();
	}
	else
		std::cout << "Unrecoverable error while opening config file" << std::endl;
	return;
}

Server::~Server( void )
{
	this->closefds();
}

void	Server::launch( void )
{
	this->_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverfd == -1)
		throw Server::ErrnoException(); 
	std::cout << "Launching Server..." << std::endl;
	std::memset(&this->_server_addr, 0, sizeof(this->_server_addr));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(PORT);
	this->_socklen = sizeof(this->_server_addr);
	std::cout << "Server launched" << std::endl;
	if (bind(this->_serverfd, (const struct sockaddr *)&(this->_server_addr), sizeof(this->_server_addr)) == -1)
		throw Server::ErrnoException(); 
	std::cout << "Server bound to address and port: " << PORT << std::endl;
}

void	Server::listen_poll( void )
{
	if (listen(this->_serverfd, MAX_CONNECTIONS) == -1)
		throw Server::ErrnoException(); 
	std::cout << "Server is ready to listen" << std::endl;
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		this->_slots[i] = false;
	this->_pfd[0].fd = this->_serverfd;
	this->_pfd[0].events = POLLIN;
	this->_slots[0] = true;
	std::cout << "Server is listening and starts polling" << std::endl;
	while (Server::signal == false)
	{
		if (poll(this->_pfd, 200, -1) == -1)
			throw Server::ErrnoException(); 
		for (long unsigned int i = 0; i <= this->_connections.size(); i++) // monitors all connected clients sockets for their commands
		{
			if (this->_pfd[i].revents & POLLIN)
			{
				if (i == 0)
					this->addClient();
				else
					this->_connections[i]->ReceiveInput();
			}
		}
	}
	this->closefds();
}

void	Server::closefds( void )
{
	close(this->_serverfd);
	std::cout << "Server has been properly shutdown" << std::endl;
}

/* Internal Functions to add/remove clients within the list of connections */
void	Server::addClient( void )
{
	Client	*c;

	int	slot = this->getFirstSlot();
	c = new Client(this, slot);
	this->_pfd[slot].fd = c->getClientfd();
	this->_pfd[slot].events = POLLIN;
	this->_connections.insert(std::make_pair(slot, c));
	std::cout << "A new Client was added" << std::endl;
}

void	Server::removeClient( const Client *client )
{
	std::map<int, Client *>::iterator it;
	it = _connections.find(client->getSlot());
	if (it != _connections.end())
	{
		close(client->getClientfd());
		delete it->second;
		_connections.erase(it);
		std::cout << "Client was disconnected" << std::endl;
	}
}

/* Setters, Getters and private functions to manage available slot list */
int	Server::getFirstSlot( void )
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (this->_slots[i] == false)
		{
			this->_slots[i] = true;
			return i;
		}
	}
	//if max connections is reached?
	return (-1);
}

void	Server::setFreeSlot( const int i )
{
	this->_slots[i] = false;
	//if i > max connections?
}

void	Server::setBusySlot( const int i)
{
	this->_slots[i] = true;
	//if i > max connections?
}

/*Getters & Setters */
int			Server::getFd() const
{
	return (this->_serverfd);
}

std::string	Server::getName() const
{
	return (this->_name);
}

std::string	Server::getLaunchTime() const
{
	std::string str;

	str = _launchtime;
	return (str);
}

std::string	Server::getVersion() const
{
	return (this->_version);
}

t_set		Server::getSettings() const
{
	return (this->_settings);
}

/* Management of nickname list on server */
bool Server::InsertNick(std::string &nick)
{
	//insert in std::set return a std::pair, with first element pointing to the
	//new element inserted or the duplicate element. The second element of the
	//pair is false if it was a duplicate or true if it was inserted.
	return (this->_nicknames.insert(nick).second);
}

/* Management of channel list on server */
bool	Server::isChannelExist(std::string &name)
{
	std::map<std::string, Channel *>::iterator it;
	it = _channels.find(name);
	if (it == this->_channels.end())
		return false;
	return true;
}

/*Channel	*Server::createChannel(std::string &name)
{
	Channel *c = nullptr;
	if (this->isChannelExist(name) == false)
	{
		c = new Channel(name);
		this->_channels.insert(std::make_pair(name, c));
	}
	return c;			
}*/

Channel	*Server::getChannel(std::string &name)
{
	/*checks if the channel is already listed on the server list
	 * and it not, creates it - are there cases where it should not be created? */

	Channel *c;

	std::map<std::string, Channel *>::iterator it;
	it = _channels.find(name);
	if (it != this->_channels.end())
		return it->second;
	else
	{
		c = new Channel(name);
		this->_channels.insert(std::make_pair(name, c));
	}
	return c;			
}

/* Password check */
bool	Server::checkPass(const std::string &pass) const
{
	if (pass.compare(this->_password) == 0)
		return true;
	return false;
}

/*std::vector<Channel*> Server::getChannelsForClient(const Client *client) const
{
	std::vector<Channel*> result;
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it !=_channels.end(); ++it)
	{
		if (it->second->isMember(const_cast<Client*>(client)))
			result.push_back(it->second);
	}
	return result;
}*/

void Server::removeClient(Client *client)
{
	//remove from all channels
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		it->second->removeMember(client);

	//remove from connections map
	for (std::map<int, Client*>::iterator it = _connections.begin(); it != _connections.end(); ++it)
	{
		if (it->second == client)
		{
			_connections.erase(it);
			break;
		}
	}
	delete client; //check if it doesn't cause segfault/double free
}
