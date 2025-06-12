/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/12 17:40:10 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6667
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <bits/stdc++.h>

class Server{
	public:
		Server ( void );
		virtual ~Server( void );
		Server ( const Server &s );
		Server &operator=( const Server &s );

	private:
		int			serverfd, clientfd;
		socklen_t	socklen;
		
		struct sockaddr_in	server_addr;


};

#endif
