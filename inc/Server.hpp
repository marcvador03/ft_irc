/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/03 17:12:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6667
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <bits/stdc++.h>
# include <sys/poll.h>
# include "Client.hpp"

/*
**`Server.hpp` / `Server.cpp`**  
- Initialize listening sockets (IPv4/IPv6).  
- Store and manage all connected `Client` objects.  
- Dispatch incoming data (commands) to the right handlers.  
- Provide methods for channel creation/deletion.  
*/

class Client;

class Server {
	public:
		Server ( int timeout );
		virtual ~Server( void );
		
		std::vector<Client *> 	connections;
		struct pollfd			fds[200];

		void	addClient ( void );
		void	removeClient ( void );
		int		getTimeOut ( void ) const;


	protected:
		nfds_t					_serverfd;

		
	private:
		Server ( void );
		Server ( const Server &other );
		Server &operator-( const Server &other );

		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		int						_timeout;
};

#endif
