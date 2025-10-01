/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:19:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/01 17:40:35 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP
# include <sys/socket.h>
# include <bits/stdc++.h>
# include "ft_irc.h"
# include "Client.hpp"
# include <utility>

class Client;
class Channel;

class Reply {
	public:
		Reply ( Client & );
		Reply ( Client &, const std::string &src );
		Reply ( Client &, Client & );
		Reply ( Client &, const std::string &src, Client & );
		Reply ( Client &, Channel &, const char audience, const char skip);
		Reply ( Client &, const std::string &, const char audience, const char skip);
		Reply ( Client &, const std::string &src, Channel &,  const char audience, const char skip );
		~Reply ( void );
		Reply ( const Reply & );
		Reply &operator=( const Reply & );

		void	ship( void );
		void	ship( const int );
		void	ship( const std::string & );
		void	ship( const char * );
		void	list( const int num );
		void	list( const size_t num );
		void	list( const std::string & );
		void	list( const bool b);
		void	list( const char *);

		std::string				getSrc( void ) const;
		std::deque<std::string>	getCmdList( void ) const;
		std::map<int, Client *>	getClientList( void ) const;
		bool					getSkipSender( void ) const;
		int						getSenderSlot( void ) const;

	private:
		Reply ( void );
		std::deque<std::string>		_cmdlist;
		std::map<int, Client *>		_listclients;
		std::string					_src;
		bool						_skipSender;
		int							_senderSlot;
};
#endif

