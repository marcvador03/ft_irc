/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:20:44 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/06 15:48:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <cerrno>
# include <sys/socket.h>
# include "Server.hpp"

/*
**`Client.hpp` / `Client.cpp`**  
- Store client socket, nickname, username, operator status, etc.  
- Track which channel(s) this client is in.  
- Handle raw input parsing (buffering, partial messages).  
- Implement commands like `NICK`, `USER`, `JOIN`, `PRIVMSG`.  
*/

class Client {
	public:
		/* Coplien form */	
		Client ( nfds_t serverfd, int slot );
		~Client ( void );

		/* Method to receive bytes from client socket */
		void	ReceiveInput();
		
		/* Setters & Getters */
		int		getClientfd( void ) const;
		int		getSlot( void ) const;
		
		/* Exceptions messages */
		class ErrnoException: public std::exception {
			public:
				virtual const char* what() const throw()
				{ return std::strerror(errno); }
		};

	private:
		/* Coplien form - unauthorized constructors */	
		Client ( void );
		Client ( const Client &other );
		Client &operator=( const Client &other);
		
		int	_serverfd; //fd of server connected to
		int	_clientfd; //fd of the client
		int	_slot; //slot number [pollfd array position]
  
		/* Internal variables for socket client management */
		socklen_t			_socklen;
		struct sockaddr_in	_client_addr;

};

#endif
