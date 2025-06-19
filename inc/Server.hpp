/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/19 15:29:15 by mpietrza         ###   ########.fr       */
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

/*
**`Server.hpp` / `Server.cpp`**  
- Initialize listening sockets (IPv4/IPv6).  
- Store and manage all connected `Client` objects.  
- Dispatch incoming data (commands) to the right handlers.  
- Provide methods for channel creation/deletion.  
*/


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
