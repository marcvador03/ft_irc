/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:19:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/19 12:37:29 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP
# include <sys/socket.h>
# include <bits/stdc++.h>
# include "ft_irc.h"

class Client;

class Reply {
	public:
		Reply ( Client & );
		Reply ( Client &, const std::string &src );
		~Reply ( void );
		Reply ( const Reply & );
		Reply &operator=( const Reply & );

		void	ship( void );
		void	ship( const int );
		void	ship( const std::string &);
		void	ship( const char *);
		void	list( const int num );
		void	list( const size_t num );
		void	list( const std::string &);
		void	list( const bool b);
		void	list( const char *);

		int							getFd( void ) const;
		std::string					getSrc( void ) const;
		std::deque<std::string>	getCmdList( void ) const;

	private:
		Reply ( void );
		std::deque<std::string>		_cmdlist;
		int							_clientfd;
		std::string					_src;
};
#endif

