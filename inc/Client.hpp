/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:20:44 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/04 16:43:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
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
		Client ( nfds_t serverfd, int slot );
		~Client ( void );
		
		void	ReceiveInput();
		nfds_t	getClientfd( void ) const;
		int		getSlot( void ) const;

	private:
		Client ( void );
		Client ( const Client &other );
		Client &operator=( const Client &other);
		
		nfds_t				_serverfd;
		nfds_t				_clientfd;
		int					_slot;
		socklen_t			_socklen;
		struct sockaddr_in	_client_addr;

		std::string			_buffer_string;
};

#endif
