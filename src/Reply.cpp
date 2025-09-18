/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:08:11 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 12:04:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Reply.hpp"

Reply::Reply ( const int fd, const std::string &src ): 
	_clientfd(fd),
	_src(src)
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

void	Reply::ship( void ) const
{
	std::vector<std::string>::const_iterator it;
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
	
	std::stringstream ss;
	ss << _clientfd;
	log("Reply: " + str + " sent to "+ ss.str());
}

int							Reply::getFd( void ) const
{
	return _clientfd;
}

std::vector<std::string>	Reply::getCmdList( void ) const
{
	return _cmdlist;
}

std::string					Reply::getSrc( void ) const
{
	return _src;
}
