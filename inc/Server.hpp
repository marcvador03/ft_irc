/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/02 13:38:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define PORT 6667
# define MAX_CONNECTIONS 1000
# include <ctime>
# include <fstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <bits/stdc++.h>
# include <sys/poll.h>
# include "ft_irc.h"
# include "Client.hpp"
# include "Channel.hpp"

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
		Server ( const std::string &servername, const std::string &pass );
		virtual ~Server( void );

		/* Server launch sequences */
		void	launch( void ); // initiate socket server and binds it to given port
		void	listen_poll( void ); // sets the server in listening mode and keeps polling inputs
		void	closefds( void ); // sweep function to properly close all sockets
	
		static bool	signal;
		t_arg 		args;
		
		/* Method to receive bytes from client socket */
		void	ReceiveInput(Client *);
		void	LaunchCmd(Client *);

		/*Getters & Setters */
		int				getFd() const;	
		std::string		getName() const;	
		std::string		getLaunchTime() const;	
		std::string		getVersion() const;

		/*Settings getters & setters*/	
		t_settings		getSettings() const;	
		void			setSettings(const char *);
		std::string		getSetting(const std::string) const;
		
		/* Functions to add/remove clients within the list of connections */
		void	addClient ( void );
		void	removeClient ( const Client *client );
		Client	&getClient ( const std::string & );
		bool	isClientExist(const std::string &name);
		std::map<int, Client *>	&getAllClients( void );
		
		/* Management of nickname list on server */
		bool 	InsertNick(std::string &nick);

		/* Management of channel list on server */
		bool	isChannelExist(std::string &name);
		Channel	*getChannel( const std::string &name, Client & );	
		Channel	*getChannel( const std::string &name );	
		//std::vector<Channel*> getChannelsForClient(const Client *client) const;
		
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
		
		std::string _trim (const std::string &str);
		
		/* Server variables */
		std::string			_name; //server name
		std::string			_password; // server password
		int					_serverfd; // fd of the server
		std::map<int, bool>	_slots; // list slots for pollfd and status false: free to accept new client true: occupied by a client
		char				_launchtime[100];
		std::string			_version;
	
		/* Variables and methods related to Settings*/	
		t_settings			_settings; //RPL_ISUPORT parameters
		std::vector<char>	_chantags;
		void				_setChanPrefix( void );
		
		/* List of connections and pollfd structure array */
		std::map<int, Client *> 	_clients;
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
