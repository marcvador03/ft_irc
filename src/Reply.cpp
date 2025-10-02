/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:08:11 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/02 13:59:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/Reply.hpp"

/*Constructors*/
Reply::Reply ( Client &c, const std::string &src, Channel &chan, const char audience, const char skipsender ): 
	_src(src),
	_senderSlot(c.getSlot())
{
	switch (audience) {
		case 'a':
			_listclients = chan.getAllClients();
			break;
		case 'o':
			_listclients = chan.getOpsClients();
			break;
		case 's':
			_listclients = c.getServerClientList();
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
	switch (skipsender) {
		case 'y':
			_skipSender = true;
			break;
		case 'n':
			_skipSender = false;
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
	
}

Reply::Reply ( Client &c, Channel &chan, const char audience, const char skipsender ): 
	_src(c.getServername()),
	_senderSlot(c.getSlot())
{
	switch (audience) {
		case 'a':
			_listclients = chan.getAllClients();
			break;
		case 'o':
			_listclients = chan.getOpsClients();
			break;
		case 's':
			_listclients = c.getServerClientList();
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
	switch (skipsender) {
		case 'y':
			_skipSender = true;
			break;
		case 'n':
			_skipSender = false;
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
}

Reply::Reply ( Client &c, const std::string &src, const char audience, const char skipsender ): 
	_src(src),
	_senderSlot(c.getSlot())
{
	switch (audience) {
		case 's':
			_listclients = c.getServerClientList();
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
	switch (skipsender) {
		case 'y':
			_skipSender = true;
			break;
		case 'n':
			_skipSender = false;
			break;
		default:
			std::cout << "error in Reply constructor" << std::endl;
	}
}

Reply::Reply ( Client &c, const std::string &src ): 
	_src(src),
	_skipSender(false),
	_senderSlot(c.getSlot())
{
	_listclients[c.getSlot()] = &c;
}

Reply::Reply ( Client &c ): 
	_src(c.getServername()),
	_skipSender(false),
	_senderSlot(c.getSlot())
{
	_listclients[c.getSlot()] = &c;
}

Reply::Reply ( Client &c, const std::string &src, Client &receiver ): 
	_src(src),
	_skipSender(false),
	_senderSlot(c.getSlot())
{
	_listclients[c.getSlot()] = &receiver;
}

Reply::Reply ( Client &c, Client &receiver ): 
	_src(c.getServername()),
	_skipSender(false),
	_senderSlot(c.getSlot())
{
	_listclients[c.getSlot()] = &receiver;
}

Reply::~Reply ( void )
{
	_cmdlist.erase(_cmdlist.begin(), _cmdlist.end());
	_listclients.erase(_listclients.begin(), _listclients.end());
}

/* Copy constructors*/
Reply::Reply ( const Reply &o ): 
	_cmdlist(o.getCmdList()),
	_listclients(o.getClientList()),
	_src(o.getSrc()),
	_skipSender(o.getSkipSender()),
	_senderSlot(o.getSenderSlot())
{
}

Reply &Reply::operator=( const Reply &other )
{
	if (this != &other)
	{
		_cmdlist = other.getCmdList();
		_src = other.getSrc();
		_listclients = other.getClientList();
		_skipSender = other.getSkipSender();
	}
	return *this;	
}

/*List methods*/
void	Reply::list( const std::string &str)
{
	_cmdlist.push_back(str);
}

void	Reply::list( const char *str)
{
	_cmdlist.push_back(static_cast<std::string>(str));
}

void	Reply::list( const bool b)
{
	std::string	str;

	str = b ? "true" : "false";
	_cmdlist.push_back(str);
}

void	Reply::list( const size_t num )
{
	std::string	str;
	std::stringstream ss;

	ss << num;
	if (num > 999)
	{
		std::cout << "Error in reply command, too many numeric" << std::endl;
		return;
	}
	else if (num < 10)
		str += "00" + ss.str();
	else if (num < 100)
		str += "0" + ss.str();
	else
		str += ss.str();
	_cmdlist.push_back(str);
}

void	Reply::list( const int num )
{
	std::string	str;
	std::stringstream ss;

	ss << num;
	if (num < 0 || num > 999)
	{
		std::cout << "Error in reply command, too many numeric" << std::endl;
		return;
	}
	else if (num < 10)
		str += "00" + ss.str();
	else if (num < 100)
		str += "0" + ss.str();
	else
		str += ss.str();
	_cmdlist.push_back(str);
}

void	Reply::ship( const int num )
{
	std::string	str;
	std::stringstream ss;

	ss << num;
	if (num < 0 || num > 999)
	{
		std::cout << "Error in reply command, too many numeric" << std::endl;
		return;
	}
	else if (num < 10)
		str += "00" + ss.str();
	else if (num < 100)
		str += "0" + ss.str();
	else
		str += ss.str();
	_cmdlist.push_front(str);
	this->ship();
}

void	Reply::ship( const std::string &msg )
{
	_cmdlist.clear();
	_cmdlist.push_back(msg);
	this->ship();
}

void	Reply::ship( const char *msg )
{
	_cmdlist.erase(_cmdlist.begin(), _cmdlist.end());
	_cmdlist.push_back(static_cast<std::string>(msg));
	this->ship();
}

void	Reply::ship( void )
{
	std::deque<std::string>::const_iterator it;
	std::string	str;
	std::map<int, Client *>::iterator it2;

	str = ":" + _src + " ";
	it = _cmdlist.begin();
	for (it = _cmdlist.begin(); it != _cmdlist.end(); it++)
	{	
		if ((it + 1) == _cmdlist.end())
			str += ":" + *it + " ";
		else
			str += *it + " ";
	}
	str += "\r\n";

	for (it2 = _listclients.begin(); it2 != _listclients.end(); it2++)
	{
		if (_skipSender == false || (_skipSender == true && it2->second->getSlot() != _senderSlot))
		{
			std::stringstream ss;
			send(it2->second->getClientfd(), str.c_str(), str.length(), 0);
			ss << it2->second->getClientfd();
			std::cout << "Reply sent to " << it2->second->getNickname();
			std::cout << "|" << str.c_str();
		}
	}
	_cmdlist.erase(_cmdlist.begin(), _cmdlist.end());
	
}

std::deque<std::string>	Reply::getCmdList( void ) const
{
	return _cmdlist;
}

std::string				Reply::getSrc( void ) const
{
	return _src;
}

std::map<int, Client *>	Reply::getClientList( void ) const
{
	return _listclients;
}

bool					Reply::getSkipSender( void ) const
{
	return _skipSender;
}

int						Reply::getSenderSlot( void ) const
{
	return _senderSlot;
}
