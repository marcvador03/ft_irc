/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:13:42 by mpietrza          #+#    #+#             */
/*   Updated: 2025/09/30 15:59:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "ft_irc.h"
# include "Client.hpp"

/*
**`Channel.hpp` / `Channel.cpp`**  
- Store channel name, topic, operators, invited users, etc.  
- Manage channel modes (`i`, `t`, `k`, `o`, `l`).  
- Forward messages to all users in channel.  
- Implement commands like `KICK`, `INVITE`, `TOPIC`, `MODE`.  
 */

class Client;

class Channel {
	public:
		/* Coplien form */	
		Channel ( const std::string name, Client * );
		~Channel ( void );

		/*Getters and setters */
		std::string				getName( void ) const;
		std::string				getFullName( void ) const;
		std::string				getCreationTime( void ) const;
		std::map<int, Client *>	&getAllClients( void );
		std::map<int, Client *>	&getOpsClients( void );
		
		//membership management
		void addMember( Client * );
		void removeMember( Client * );
		bool isMember( Client * );
		
		//key (password) related
		int		setKey(const std::string &key);
		void	unsetKey( void );
		bool 	checkKey(const std::string &key) const;
		bool 	hasKey(void) const;
		
		//limit related
		void	setLimit( size_t limit );
		void	unsetLimit( void );
		size_t	getLimit( void ) const;
		bool	hasReachedLimit( void ) const;
		
		//invite only related
		void	setInviteOnly( bool );
		bool 	isInviteOnly( void ) const;
		
		//topic related
		std::string	getTopic( void ) const;
		void 		setTopic( const std::string & );
		void 		setTopicLocked( bool );
		bool 		isTopicLocked( void ) const;

		//operator management
		void addOperator(Client * );
		void removeOperator(Client * );
		bool isOperator(Client * );
		
		//broadcast messages in channel
		/*template<typename T>
		void broadcast( const Reply &rpl, T val , bool ops_only )
		{
			std::map<int, Client *> &list = (ops_only = true) ? _operators: _clients;
			for (std::map<int, Client *>::iterator it = list.begin(); it != list.end(); ++it)
			{
				if (it->second->getNickname() != rpl.getSrc())
					rpl.ship(val);
			}
		};

		void broadcast_ops(const::std::string &, const std::string );*/
		/* Exceptions messages */
		class ErrnoException: public std::exception {
			public:
				virtual const char* what() const throw()
				{ return std::strerror(errno); }
		};
	private:
		/* Coplien form - unauthorized constructors */	
		Channel ( const Channel &other );
		Channel &operator-( const Channel &other );
		
		std::map <int, Client *>	_clients;
		std::map <int, Client *>	_operators;		//channel operators (for mode 'o')

		std::string	_name;			//channel name
		std::string	_topic;			//channel topic
		bool		_inviteOnly;	//mode 'i' - new members can only join when inviteed by other users (true) / everyone can join (false)
		bool		_topicLocked;	//mode 't' - topic of the channel can (false) or cannot be changed (true)
		bool		_hasKey;		//mode 'k' - the channel has key (password) (true) or doesn't have it (false)
		std::string	_key;			//mode 'k''s key (password) storage
		bool		_hasLimit;		//mode 'l' - flag for the limit - false = no limit set on the channel
		size_t		_limit;			//mode 'l' - the channel doesn't have any limit of users (0) or has a limit of users within the range of unsigned int (1 - 4,294,967,295)
		time_t		_creationtime;
};

#endif
