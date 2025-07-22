/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:20:44 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/22 19:10:16 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <string>
# include <iterator>
# include <cerrno>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <bits/stdc++.h>
# include <map>
# include "Channel.hpp"
# include "Server.hpp"

/*
**`Client.hpp` / `Client.cpp`**  
- Store client socket, nickname, username, operator status, etc.  
- Track which channel(s) this client is in.  
- Handle raw input parsing (buffering, partial messages).  
- Implement commands like `NICK`, `USER`, `JOIN`, `PRIVMSG`.  
*/

//`t_arg` is a type alias for a map that associates an integer (slot) with
//a string (argument). This is used to store command arguments for the client.
typedef std::map<int, std::string> t_arg;
typedef std::map<std::string, std::string> t_list;

class Channel;
class Server;

class Client {
	public:
		/* Coplien form */	
		Client ( Server *s, int slot );
		~Client ( void );
		
		t_arg 		args;
		
		/* Method to receive bytes from client socket */
		void	ReceiveInput();
		void	LaunchCmd();
		void 	reply(const std::string& msg);
		
		/* Setters & Getters */
		int			getClientfd( void ) const;
		int			getSlot( void ) const;
		std::string	getNickname( void ) const;
		int			setNickname( std::string & );
		std::string	getName( void ) const;
		void		setName( std::string &name);
		int			leaveChannel( std::string );
		int			leaveAllChannels( void );
		int			joinChannel( std::string, std::string );
		bool		isPartofChannel( std::string &name);

		public:
			class ErrnoException: public std::exception {
				virtual const char* what() const throw() 
				{return std::strerror(errno);}
			};
	
	private:
		/* Coplien form - unauthorized constructors */	
		Client ( void );
		Client ( const Client &other );
		Client &operator=( const Client &other);

		Server		*_server;
		
		int			_clientfd; //fd of the client
		int			_slot; //slot number [pollfd array position]
		std::string	_name;			//client name
		std::string	_nickname;		
		size_t		_chanlim;
  
		/* Internal variables for socket client management */
		socklen_t			_socklen;
		struct sockaddr_in	_client_addr;
	
		std::set<Channel *> _channels; //channels which client is member of
};

// Command handlers
void handleJoin( Client &c );
void handlePing( Client &c );
void handleNick( Client &c ); 
#endif
