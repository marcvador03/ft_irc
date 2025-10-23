/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:31:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/23 11:46:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"
#include <cctype> // for toupper
#include <climits> // for INT_MAX

Server::Server ( const std::string &servername, const std::string &pass ): // we will need to update default inputs with program argv!! <-done
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
	std::map<int, Client *>::iterator 			it;
	std::map<std::string, Channel *>::iterator 	it2;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{		
		it->second->rpl_UnexpectedQuit("Server was shut");
		delete it->second;
	}
	for (it2 = _channels.begin(); it2 != _channels.end(); it2++)
		delete it2->second;
	this->closefds();
}

/* Server launch sequences */

void	Server::launch( void )
{
	this->_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverfd == -1)
		throw Server::ErrnoException(); 
	std::cout << "Launching Server..." << std::endl;
	std::memset(&this->_server_addr, 0, sizeof(this->_server_addr));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_port);	
	this->_socklen = sizeof(this->_server_addr);
	std::cout << "Server launched" << std::endl;
	int	reuse = 1;
	if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) == -1)
		throw Server::ErrnoException(); 
	if (bind(this->_serverfd, (const struct sockaddr *)&(this->_server_addr), sizeof(this->_server_addr)) == -1)
		throw Server::ErrnoException(); 
	std::cout << "Server bound to address and port: " << this->_port << std::endl;
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
}

void	Server::closefds( void )
{
	close(this->_serverfd);
	std::cout << "Server file descriptor has been properly shutdown" << std::endl;
}

/* Method to receive bytes from client socket */

void	Server::ReceiveInput(Client *c)
{
	char		buf[BUF_SIZE];
	int			bytes;

	std::memset(buf, 0, sizeof(buf));
	bytes = recv(c->getClientfd(), buf, sizeof(buf) - 1, 0);
	if (bytes == -1)
		throw Server::ErrnoException(); 
	else if (bytes == 0)
		return (removeClient(c));
	else
	{
		c->incrementBuffer(buf, bytes);
		if (c->readBuffer().rfind('\n') == c->readBuffer().npos)
			return;
		std::string	str = c->getLineBuffer();
		std::cout << "Received input from " << c->getNickname();
		std::cout << "|" << str;
		std::istringstream ss(str);
		for (std::string line; std::getline(ss, line);) 
		{
			line = trimstr(line);
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
			if (!args[0].empty())
				toupper(args[0]); //capitalizing command token only
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
	else if (args[0] == "KICK")
		c->handleKick(args);
	else if (args[0] == "INVITE")
		c->handleInvite(args);
	else if (args[0] == "TOPIC")
		c->handleTopic(args);
	else if (args[0] == "WHO")
		c->handleWho(args);
	else if (args[0] == "WHOIS")
		c->handleWhoIs(args);
	else if (args[0] == "AWAY")
		c->handleAway(args);
	else if (args[0] == "LIST")
		c->handleList(args);
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

void	Server::setPort(const int port)
{
	this->_port = port;
}

/*Settings getters & setters*/	

std::string		Server::getSetting(const std::string str) const
{
	for (size_t i = 0; i < _settings.size();i++)
	{
		if (_settings[i].find(str) != _settings[i].end())
			return _settings[i].find(str)->second;
	}
	return "";
}

t_settings	Server::getSettings() const
{
	return (this->_settings);
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
		_parseMaxtargets();
		_parseTargmax();
	}
	else
		throw std::runtime_error("Could not open setting file");
}

size_t			Server::getChanLim() const
{
	std::vector<std::string>	str;
	std::map<char, int>		chanset;

	str = split(getSetting("CHANLIMIT"), ',');
	for (std::vector<std::string>::iterator it = str.begin(); it != str.end(); it++)
	{
		if ((*it).find(":") != (*it).npos)
		{
			for (size_t i = (*it).find(":") + 1; i < (*it).size();i++)
			{
				if (std::isdigit((*it)[i]) == false)
					return 1000;	
			}
			size_t n = std::atoi((*it).substr((*it).find(":") + 1, (*it).size()).c_str());
			for (size_t i = 0; i != (*it).find(":"); i++)
			{
				if ((*it)[i] == '#')
					return n;
			}
		}
	}
	return 1000;
}

size_t Server::getTargmax() const
{
	return _targmaxDefault;
}

size_t Server::getTargmax(const std::string &cmd) const
{
	std::string key = trimstr(cmd); //utils.hpp
	toupper(key); //utils.hpp
	
	std::map<std::string, size_t>::const_iterator it = _targmaxPerCommand.find(key);
	if (it != _targmaxPerCommand.end())
		return it->second;
	return _targmaxDefault;
}

int Server::getLen(const std::string key, const std::string keyFullName, int stdLen) const
{
	long lenLong;
	std::istringstream(Server::getSetting(key)) >> lenLong;
	
	if (lenLong < 1 || lenLong > INT_MAX)
	{
		std::cout << "Error! " << keyFullName << " lenght limit given in irc_config is not a valid number!" << std::endl
				  << "Setting standard value of " << stdLen << " characters." << std::endl;
		return stdLen;
	}
	return static_cast<int>(lenLong);
}

/* Internal Functions to add/remove clients within the list of connections */
void	Server::addClient( void )
{
	Client	*c;

	int	slot = this->_getFirstSlot();
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
		close(client->getClientfd());
		_setFreeSlot(client->getSlot());
		removeNick(client->getNickname());
		delete it->second;
		_clients.erase(it);
		std::cout << "Client was disconnected" << std::endl;
	}
}
		
bool	Server::isClientExist(const std::string &name)
{
	const std::string normalizedNick = _casefoldNick(name);
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (_casefoldNick(it->second->getNickname()) == normalizedNick)
			return true;
	}
	return false;
}

std::map<int, Client *>	&Server::getAllClients( void )
{
	return _clients;
}

Client	&Server::getClient ( const std::string &name )
{
	const std::string normalizedNick = _casefoldNick(name);
	std::map<int, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (normalizedNick.compare(_casefoldNick(it->second->getNickname())) == 0)
			return *it->second;
	}
	return *it->second;
}

/* Management of nickname list on server */

bool 	Server::InsertNick(const std::string &nick)
{
	// store the folded form to enforce case-insensitive uniqueness
	return this->_nicknames.insert(_casefoldNick(nick)).second;
}

void	Server::removeNick(const std::string &nick)
{
	_nicknames.erase(_casefoldNick(nick));
	return;
}

/* Management of channel list on server */
bool	Server::isChannelExist(const std::string &name)
{
	const CaseMapping cm = parseCaseMapping(getSetting("CASEMAPPING"));
	const std::string normalizedChan = casefold(name, cm);
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (casefold(it->first, cm) == normalizedChan)
			return true;
	}
	return false;
}

Channel	*Server::getChannel(const std::string &name, Client &c)
{
	const CaseMapping cm = parseCaseMapping(getSetting("CASEMAPPING"));
	const std::string key = casefold(name, cm);

	// try case-insensitive match
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (casefold(it->first, cm) == key)
			return it->second;
	}

	// not found: create with original casing
	Channel *chan = new Channel(this, name, c);
	_channels.insert(std::make_pair(name, chan));
	return chan;
}

Channel	*Server::getChannel(const std::string &name)
{
	const CaseMapping cm = parseCaseMapping(getSetting("CASEMAPPING"));
	const std::string key = casefold(name, cm);
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (casefold(it->first, cm) == key)
			return it->second;
	}
	return NULL;
}

std::map<std::string, Channel *> Server::getAllChannels( void )
{
	return _channels;
}

std::vector<Channel *> Server::getChannelsforClient( Client & client)
{
	std::vector<Channel *> result;
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it !=_channels.end(); ++it)
	{
		if (it->second->isMember(client))
			result.push_back(it->second);
	}
	return result;
}

void	Server::deleteChannel(Channel *chan)
{
	std::string name;
	
	if (chan == NULL)
		return;
	name = chan->getName();
	delete _channels.find(name)->second;
	_channels.erase(_channels.find(name));
	return ;
}

/* Password check */
bool	Server::checkPass(const std::string &pass) const
{
	if (pass.compare(this->_password) == 0)
		return true;
	return false;
}


/*PRIVATE:*/
/* Setters, Getters and private functions to manage available slot list */
int	Server::_getFirstSlot( void )
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

void	Server::_setFreeSlot( const int i )
{
	this->_slots[i] = false;
	//if i > max connections?
}

void	Server::_setBusySlot( const int i)
{
	this->_slots[i] = true;
	//if i > max connections?
}

void	Server::_setChanPrefix( void )
{
	std::string	str;

	str = getSetting("STATUSMSG");
	_chantags.clear();
	if (str.empty() == true)
		return;	
	for (size_t i = 0; i < str.size();i++)
		_chantags.push_back(str[i]);
}

void Server::_parseMaxtargets()
{
	long lenLong;
	std::istringstream(Server::getSetting("MAXTARGETS")) >> lenLong;

	//long lenLong = strtol(Server::getSetting("MAXTARGETS").c_str(), &endptr, 10);
	if (lenLong < 1 || lenLong > INT_MAX)
	{
		std::cout << "Error! Maxtargets limit given in irc_config is not a valid number!" << std::endl
				  << "Setting unlimited number of targets." << std::endl;
		_maxtargets = 0;
		return;
	}
	_maxtargets = static_cast<size_t>(lenLong);
}

static size_t strToSizeT( const std::string& str )
{
	char *endptr;
	long valueLong = strtol(str.c_str(), &endptr, 10);
	if (*endptr != '\0' || valueLong < 1 || valueLong > INT_MAX)
		return 0;
	return static_cast<size_t>(valueLong);
}

void Server::_parseTargmax()
{
	_targmaxDefault = _maxtargets; // default to server-wide max targets

	const std::string targmaxSetting = getSetting("TARGMAX");
	if (targmaxSetting.empty())
		return;

	std::vector<std::string> entries = split(targmaxSetting, ',');
	for (std::vector<std::string>::iterator it = entries.begin(); it != entries.end(); ++it)
	{
		std::string entry = trimstr(*it);
		size_t colonPos = entry.find(':');
		if (colonPos == std::string::npos)
			continue; // malformed entry, skip

		std::string cmd = trimstr(entry.substr(0, colonPos));
		std::string valueStr = trimstr(entry.substr(colonPos + 1));

		size_t value = strToSizeT(valueStr);
		if (value == 0)
		{
			std::cout << "Error! Targmax value for command " << cmd << " is not a valid number!" << std::endl
					  << "Skipping this entry." << std::endl;
			continue;
		}
		_targmaxPerCommand[cmd] = value;
	}
}

// Wrapper that uses utils::casefold according to ISUPPORT CASEMAPPING
std::string Server::_casefoldNick(const std::string& s) const
{
	const CaseMapping cm = parseCaseMapping(getSetting("CASEMAPPING"));
	return casefold(s, cm);
}
