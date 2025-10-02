/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:20:44 by mpietrza          #+#    #+#             */
/*   Updated: 2025/10/02 10:17:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <bits/stdc++.h>
# include "ft_irc.h"
# include "Channel.hpp"
# include "Server.hpp"
# include "Reply.hpp"

/*
**`Client.hpp` / `Client.cpp`**  
- Store client socket, nickname, username, operator status, etc.  
- Track which channel(s) this client is in.  
- Handle raw input parsing (buffering, partial messages).  
- Implement commands like `NICK`, `USER`, `JOIN`, `PRIVMSG`.  
*/

class Channel;
class Server;

class Client {
	public:
		/* Coplien form */	
		Client ( Server *s, int slot );
		~Client ( void );
		
		/* Setters & Getters */
		int			getClientfd( void ) const;
		int			getSlot( void ) const;
		std::string	getServername( void ) const;
		std::string	getServerLaunchTime( void ) const;
		std::string	getServerVersion( void ) const;
		t_settings	getServerSettings( void ) const;
		std::string	getServerSetting( const std::string ) const;
		std::map<int, Client *> getServerClientList( void ) const;
		std::string	getNickname( void ) const;
		int			setNickname( std::string & );
		int			setUser( std::string &, std::string &);
		std::string	getUser( void ) const;
		int			registerPass( std::string &);
		std::string	getHost( void ) const;
		int			leaveChannel( std::string );
		int			leaveAllChannels( void );
		int			joinChannel( std::string, std::string );
		bool		isPartofChannel( std::string &name);
		bool		isPasswordAccepted( void ) const;
		bool		isRegistered( void ) const;
		
		/*Commands handle */
		void handleJoin( t_arg args );
		void handlePing( t_arg args );
		void handleNick( t_arg args ); 
		void handleUser( t_arg args ); 
		void handlePass( t_arg args ); 
		void handlePart( t_arg args ); 
		void handleQuit( t_arg args ); 
		void handlePrivMsg( t_arg args ); 
		void handleMode( t_arg args ); 
		void handleKick( t_arg args ); 
		
		/* RPL functions */
		void	rpl_Welcome( void );
		void	rpl_YourHost( void );
		void	rpl_Created( void );
		void	rpl_MyInfo( void );
		void	rpl_ISupport( void );
		void	rpl_Topic( Channel & );
		void	rpl_noTopic( Channel & );
		void	rpl_NamReply( Channel & );
		void	rpl_EndOfNames( Channel & );
		void	rpl_Away( void );
		void	rpl_ChannelModeIs( Channel &, const std::string & );
		void	rpl_CreationTime( const std::string & );

		/*ERR functions */
		void	err_NeedMoreParameters( const std::string & );
		void	err_noSuchChannel( const std::string & );
		void	err_tooManyChannels( const std::string & );
		void	err_BadChannelKey( const std::string & );
		void	err_ChannelIsFull( const std::string & );
		void	err_InviteOnlyChan( const std::string & );
		void	err_BadChanMask( const std::string & );
		void	err_notOnChannel( const std::string & );
		void	err_noTextToSend( void );
		void	err_noSuchNick( void );
		void	err_UModeUnknownFlag( void );
		void	err_ChanOPrivsNeeded( const std::string & );
		void	err_InvalidModeParam( Channel &, const std::string &, const std::string &, const std::string & );
		void	err_NoOrigin( void );
		void	err_NoNicknameGiven( void );
		void	err_ErroneusNickname( const std::string & );
		void	err_NicknameInUse( const std::string & );
		void	err_AlreadyRegistered ( void );
		void	err_PasswdMismatch ( void );

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
		bool		_isPassAccepted; // flag if client has provided correct password
		bool		_isRegistered; // flag if client is registered
		bool		_hasNick;
		bool		_hasUser;

		std::string	_realname;
		std::string	_username;
		std::string	_nickname;		
		size_t		_chanlim;
		std::string	_host;
		bool		_away;
  
		/* Internal variables for socket client management */
		socklen_t			_socklen;
		struct sockaddr_in	_client_addr;

		/* Internal helpers */
		void	_send(std::string &);
		int		_completeReg( void );
	
		std::set<Channel *> _channels; //channels which client is member of
};
#endif
