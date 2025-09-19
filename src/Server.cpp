/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/19 18:59:44 by mfleury          ###   ########.fr       */
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

	return;
}

Server::~Server( void )
{
	this->closefds();
}

void	Server::setSettings(const char *link)
{
	std::ifstream	config;
	t_list			tmp;
	std::string 	line, set, val;
	
	config.open(link);
	if (config.is_open() == true)
	{
		for (int i = 0, j = 0; std::getline(config, line) ; i++, j++)
		{
			
			if (line.find("=") == line.npos)
				tmp.insert(std::pair<std::string, std::string>(line, ""));
			else
			{	
				set = line.substr(0, line.find("="));	
				val = line.substr(line.find("=") + 1, line.size());	
				tmp.insert(std::pair<std::string, std::string>(set, val));
			}
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
		_setChanPrefix();
	}
	else
		throw std::runtime_error("Could not open setting file");

}

std::string		Server::getSetting(const std::string str) const
{
	for (size_t i = 0; i < _settings.size();i++)
	{
		if (_settings[i].find(str) != _settings[i].end())
			return _settings[i].find(str)->second;
	}
	return "";
}

void	Server::_setChanPrefix( void )
{
	std::string	str;

	str = getSetting("STATUSMSG");
	_chantags.erase(_chantags.begin(), _chantags.end());
	if (str.empty() == true)
		return;	
	for (size_t i = 0; i < str.size();i++)
		_chantags.push_back(str[0]);
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
		for (long unsigned int i = 0; i <= this->_clients.size(); i++) // monitors all connected clients sockets for their commands
		{
			if (this->_pfd[i].revents & POLLIN)
			{
				if (i == 0)
					this->addClient();
				else
					this->ReceiveInput(_clients[i]);
			}
		}
	}
	this->closefds();
}

void	Server::ReceiveInput(Client *c)
{
	char		buf[2048];
	int			bytes;

	std::memset(buf, 0, sizeof(buf));
	bytes = recv(c->getClientfd(), buf, sizeof(buf) - 1, 0);
	if (bytes == -1)
		throw Server::ErrnoException(); 
	else if (bytes == 0)
	{
		delete this;
		return;
	}
	else
	{
		buf[bytes] = '\0';
		log("Received input: ");
	   	log(buf);
		std::istringstream ss(buf);
		for (std::string line; std::getline(ss, line);) 
		{
			line = this->_trim(line);
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
			this->LaunchCmd(c);
			args.erase(args.begin(), args.end());
		}
	}
}

void	Server::LaunchCmd(Client *c)
{
	if (c->isPasswordAccepted() == false)
	{
		if (args[0] == "PASS")
			c->handlePass(args);
		else if (args[0] == "PING")
			c->handlePing(args);
		else if (args[0] == "QUIT")
			c->handleQuit(args);
		return ;
	}
	if (c->isPasswordAccepted() == true && c->isRegistered() == false)
	{
		if (args[0] == "NICK")
			c->handleNick(args);
		else if (args[0] == "USER")
			c->handleUser(args);
		else if (args[0] == "PING")
			c->handlePing(args);
		else if (args[0] == "QUIT")
			c->handleQuit(args);
		return ;
	}
	if (args[0] == "PING")
		c->handlePing(args);
	else if (args[0] == "NICK")
		c->handleNick(args);
	else if (args[0] == "JOIN")
		c->handleJoin(args);
	else if (args[0] == "USER")
		c->handleUser(args);
	else if (args[0] == "QUIT")
		c->handleQuit(args);
	else if (args[0] == "PART")
		c->handlePart(args);
	else if (args[0] == "PRIVMSG")
		c->handlePrivMsg(args);
	else if (args[0] == "MODE")
		c->handleMode(args);
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
	this->_clients.insert(std::make_pair(slot, c));
	std::cout << "A new Client was added" << std::endl;
}

void	Server::removeClient( const Client *client )
{
	std::map<int, Client *>::iterator it;
	it = _clients.find(client->getSlot());
	if (it != _clients.end())
	{
		//close(client->getClientfd());
		delete it->second;
		_clients.erase(it);
		std::cout << "Client was disconnected" << std::endl;
	}
}
		
bool	Server::isClientExist(std::string &name)
{
	std::map<int, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (name.compare(it->second->getNickname()) == 0)
			return true;
	}
	return false;

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

t_settings	Server::getSettings() const
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

Channel	*Server::getChannel(const std::string &name, Client *c)
{
	/*checks if the channel is already listed on the server list
	 * and it not, creates it - are there cases where it should not be created? */

	Channel *chan;

	std::map<std::string, Channel *>::iterator it;
	it = _channels.find(name);
	if (it != this->_channels.end())
		return it->second;
	else
	{
		chan = new Channel(name, c);
		this->_channels.insert(std::make_pair(name, chan));
	}
	return chan;			
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

/*void Server::_removeClient(Client *client)
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
}*/

std::string Server::_trim (const std::string &str) 
{
	const std::string WHITESPACE = " \n\r\t\f\v";
	size_t start = str.find_first_not_of(WHITESPACE);
	if (start == std::string::npos)
		return "";
	size_t end = str.find_last_not_of(WHITESPACE);
	return str.substr(start, end - start + 1);
}
