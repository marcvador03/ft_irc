/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/12 22:45:53 by mfleury          ###   ########.fr       */
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
# include <sys/poll.h>

class Server{
	public:
		Server ( void );
		~Server( void );
		
	private:
		int					_serverfd;
		socklen_t			_socklen;
		struct sockaddr_in	_server_addr;
		struct pollfd		_fds[200];
		int					_timeout;
};

#endif
