/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:08:11 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 13:09:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Reply::Reply ( Client &c, const std::string &src ): 
	_clientfd(c.getClientfd()),
	_src(src)
{}

Reply::Reply ( Client &c ): 
	_clientfd(c.getClientfd()),
	_src(c.getServername())
{}

Reply::~Reply ( void )
{
	_cmdlist.clear();
}

Reply::Reply ( const Reply &o ): 
	_cmdlist(o.getCmdList()),
	_clientfd(o.getFd()),
	_src(o.getSrc())
{
}

Reply &Reply::operator=( const Reply &other )
{
	if (this != &other)
	{
		_clientfd = other.getFd();
		_cmdlist = other.getCmdList();
		_src = other.getSrc();
	}
	return *this;	
}

void	Reply::list( const std::string &str)
{
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

void	Reply::ship( void )
{
	std::deque<std::string>::const_iterator it;
	std::string	str;

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
	send(_clientfd, str.c_str(), str.length(), 0);
	_cmdlist.clear();
	
	std::stringstream ss;
	ss << _clientfd;
	log("Reply: " + str + " sent to "+ ss.str());
}

int							Reply::getFd( void ) const
{
	return _clientfd;
}

std::deque<std::string>	Reply::getCmdList( void ) const
{
	return _cmdlist;
}

std::string					Reply::getSrc( void ) const
{
	return _src;
}
