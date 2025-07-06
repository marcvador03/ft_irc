/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/06 13:40:55 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6667
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
		/* Coplien form */	
		Server ( void );
		virtual ~Server( void );
		
		/* List of connections and pollfd structure array */
		std::map<int, Client *> 	connections;
		struct pollfd			pfd[MAX_CONNECTIONS];

		/* Functions to add/remove clients within the list of connections */
		void	addClient ( void );
		void	removeClient ( const Client *client );
		//int		getTimeOut ( void ) const;

	private:
		/* Coplien form - unauthorized constructors */	
		Server ( const Server &other );
		Server &operator-( const Server &other );

		/* Internal functions to manage available slots in pollfd array struct */
		int		getFirstSlot( void );
		void	setFreeSlot( const int i );
		void	setBusySlot( const int i); // unused
		
		nfds_t					_serverfd; // fd of the server
		std::map<int, bool>		_slots; // list slots for pollfd and status
										// false: free to accept new client
										// true: occupied by a client
		
		/* Internal variables for socket client management */
		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		//int						_timeout;
};

#endif
