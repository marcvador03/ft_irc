/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/04 14:32:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6668
# define MAX_CONNECTIONS 1000
# include <iostream>
# include <map>
# include <iterator>
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
		
		std::map<int, Client *> 	connections;
		struct pollfd			pfd[MAX_CONNECTIONS];

		void	addClient ( void );
		void	removeClient ( const Client *client );
		int		getTimeOut ( void ) const;

	private:
		Server ( void );
		Server ( const Server &other );
		Server &operator-( const Server &other );

		int		getFirstSlot( void );
		void	setFreeSlot( const int i );
		void	setBusySlot( const int i);

		nfds_t					_serverfd;
		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		std::map<int, bool>		_slots;
		int						_timeout;
};

#endif
