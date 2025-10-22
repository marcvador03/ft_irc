/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:13:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/22 14:25:28 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_CONNECTIONS 1000
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/poll.h>
# include "ft_irc.h"
# include "Client.hpp"
# include "Channel.hpp"

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
		void			setPort(const int port);
		
		/*Settings getters & setters*/	
		std::string		getSetting(const std::string) const;
		t_settings		getSettings() const;	
		void			setSettings(const char *);
				
		size_t			getChanLim() const;
		size_t			getTargmax() const; // default TARGMAX
		size_t			getTargmax(const std::string &cmd) const; //per-command TARGMAX
		int				getLen(const std::string key, const std::string keyFullName, int stdLen) const;

		/* Functions to add/remove clients within the list of connections */
		void					addClient ( void );
		void					removeClient ( const Client *client );
		bool					isClientExist(const std::string &name);
		std::map<int, Client *>	&getAllClients( void );
		Client					&getClient ( const std::string & );
		
		/* Management of nickname list on server */
		bool 	InsertNick(const std::string &nick);
		void 	removeNick(const std::string &nick);
		

		/* Management of channel list on server */
		bool	isChannelExist(const std::string &name);
		Channel	*getChannel( const std::string &name, Client & );	
		Channel	*getChannel( const std::string &name );	
		std::map<std::string, Channel *>
				getAllChannels( void );
		std::vector<Channel *>
				getChannelsforClient( Client & );
		void	deleteChannel(Channel *);
		
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
		int		_getFirstSlot( void );
		void	_setFreeSlot( const int i );
		void	_setBusySlot( const int i); // unused
		
		/* Server variables */
		std::string			_name; //server name
		std::string			_password; // server password
		int 				_port; // server port
		int					_serverfd; // fd of the server
		std::map<int, bool>	_slots; // list slots for pollfd and status false: free to accept new client true: occupied by a client
		char				_launchtime[100];
		std::string			_version;
		
		size_t							_maxtargets;
		size_t			 				_targmaxDefault;
		std::map<std::string, size_t>	_targmaxPerCommand;
	
		/* Variables and methods related to Settings*/	
		t_settings			_settings; //RPL_ISUPORT parameters
		std::vector<char>	_chantags;
		void				_setChanPrefix( void );
		void				_parseMaxtargets();
		void				_parseTargmax();
		
		/* List of connections and pollfd structure array */
		std::map<int, Client *> 	_clients;
		std::set<std::string>		_nicknames;
		struct pollfd				_pfd[MAX_CONNECTIONS];
		
		/* List of channels live in server */
		std::map<std::string, Channel *> 	_channels;
		
		/* Internal variables for socket client management */
		socklen_t				_socklen;
		struct sockaddr_in		_server_addr;
		
		/* Private utility functions */
		std::string _casefoldNick(const std::string& s) const;
};

#endif
