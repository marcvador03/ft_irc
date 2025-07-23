/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/23 21:22:09 by mfleury          ###   ########.fr       */
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
# include <ctime>
# include <fstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <bits/stdc++.h>
# include <sys/poll.h>
# include "Client.hpp"
# include "Channel.hpp"

/*
**`Server.hpp` / `Server.cpp`**  
- Initialize listening sockets (IPv4/IPv6).  
- Store and manage all connected `Client` objects.  
- Dispatch incoming data (commands) to the right handlers.  
- Provide methods for channel creation/deletion.  
*/
typedef std::vector<std::vector<std::string> > t_set;

class Client;
class Channel;

class Server {
	public:
		/* Coplien form */	
		Server ( const std::string &servername, const std::string &pass );
		virtual ~Server( void );

		/* Server launch sequences */
		void	launch( void ); // initiate socket server and binds it to given port
		void	listen_poll( void ); // sets the server in listening mode and keeps polling inputs
		void	closefds( void ); // sweep function to properly close all sockets
	
		static bool	signal;

		/*Getters & Setters */
		int			getFd() const;	
		std::string	getName() const;	
		std::string	getLaunchTime() const;	
		std::string	getVersion() const;	
		t_set		getSettings() const;	
		
		/* Management of nickname list on server */
		bool 	InsertNick(std::string &nick);

		/* Management of channel list on server */
		bool	isChannelExist(std::string &name);
		//Channel	*createChannel(std::string &name);	
		Channel	*getChannel(std::string &name);	
		
		/* Password check */
		bool	checkPass(const std::string &) const;
		
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
		Server ( void );
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
		std::string			_name; //server name, handpicked: .irc42
		std::string			_password; // server password
		int					_serverfd; // fd of the server
		std::map<int, bool>	_slots; // list slots for pollfd and status false: free to accept new client true: occupied by a client
		char				_launchtime[100];
		std::string			_version;
		
		t_set				_settings; //RPL_ISUPORT parameters
		
		/* List of connections and pollfd structure array */
		std::map<int, Client *> 	_connections;
		std::set<std::string>		_nicknames;
		struct pollfd				_pfd[MAX_CONNECTIONS];
		
		/* List of channels live in server */
		std::map<std::string, Channel *> 	_channels;
		
		/* Internal variables for socket client management */
		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		//int						_timeout;
};

#endif
