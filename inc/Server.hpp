/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/23 13:09:23 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6667
# define MAX_CONNECTIONS 1000
# include <iostream>
# include <map>
# include <set>
# include <iterator>
# include <cerrno>
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
class Channel;

class Server {
	public:
		/* Coplien form */	
		Server ( void );
		virtual ~Server( void );
		

		/* Server launch sequences */
		void	launch( void ); // initiate socket server and binds it to given port
		void	listen_poll( void ); // sets the server in listening mode and keeps polling inputs
		void	closefds( void ); // sweep function to properly close all sockets
	
		static bool	signal;

		/*Getters & Setters */
		int	get_Fd() const;	
		bool insert_nickname(std::string &nick);
	
		std::vector<Channel*> getChannelsForClient(const Client *client) const;
		void removeClient(Client *client);

		public:
			class ErrnoException: public std::exception {
				virtual const char* what() const throw() 
				{return std::strerror(errno);}
			};

			class ServerPortIncorrectRange: public std::exception {
				virtual const char* what() const throw()
				{return "Port xx cannot be used for IRC server";} 
			};
	private:
		/* Coplien form - unauthorized constructors */	
		Server ( const Server &other );
		Server &operator-( const Server &other );

		/* Internal functions to manage available slots in pollfd array struct */
		int		getFirstSlot( void );
		void	setFreeSlot( const int i );
		void	setBusySlot( const int i); // unused
		
		/* Internal Functions to add/remove clients within the list of connections */
		void	addClient ( void );
		void	removeClient ( const Client *client );
		
		/* Server variables */
		std::string				_password; // server password
		int						_serverfd; // fd of the server
		std::map<int, bool>		_slots; // list slots for pollfd and status
										// false: free to accept new client
										// true: occupied by a client
		
		/* List of connections and pollfd structure array */
		std::map<int, Client *> 	_connections;
		std::map<std::string, Channel*> _channels; // channels available on the server
		std::set<std::string>		_nicknames;
		struct pollfd				_pfd[MAX_CONNECTIONS];
		
		/* Internal variables for socket client management */
		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		//int						_timeout;
};

#endif
