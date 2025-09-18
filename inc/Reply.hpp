/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:19:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/18 12:04:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP
# include <sys/socket.h>
# include <bits/stdc++.h>
# include "ft_irc.h"

class Reply {
	public:
		Reply ( const int fd, const std::string &src );
		Reply ( const int num );
		Reply ( const std::string &src, const int num );
		~Reply ( void );
		Reply ( const Reply & );
		Reply &operator=( const Reply & );

		void	ship( void ) const;
		void	list( const int num );
		void	list( const std::string &);

		int							getFd( void ) const;
		std::string					getSrc( void ) const;
		std::vector<std::string>	getCmdList( void ) const;

	private:
		Reply ( void );
		std::vector<std::string>	_cmdlist;
		int							_clientfd;
		std::string					_src;
};
#endif

