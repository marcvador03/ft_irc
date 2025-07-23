/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:13:42 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/23 15:10:16 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <map>
# include <iterator>
# include "Client.hpp"
# include <string>
# include <set>
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
		Channel ( std::string name, std::string topic );
		virtual ~Channel ( void );

		void	addClient( int slot, Client *c);
		void	removeClient( int slot );
		
		/*Getters and setters */
		std::string	getName( void ) const;
		
		//*************to revist***********************//
		//invite only related
		void setInviteOnly(bool isInviteOnly);
		bool isInviteOnly() const;

		//topic related
		std::string getTopic() const;
		void setTopic(const std::string &topic);
		void setTopicLocked(bool isLocked);
		bool isTopicLocked() const;

		//key (password) related
		void setKey(const std::string &key);
		bool checkKey(const std::string &key) const;

		//limit related
		void setLimit(unsigned int limit);
		unsigned int getLimit() const;

		//membership management
		bool addMember(Client *client);
		bool removeMember(Client *client);
		bool isMember(Client *client) const;

		//operator management
		bool addOperator(Client *client);
		bool removeOperator(Client *client);
		bool isOperator(Client *client);
		
		//quit related
		void broadcast(const::std::string &message, const Client *exclude);
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

		std::string			_name;			//channel name
		std::string			_topic;			//channel topic
		bool				_inviteOnly;	//mode 'i' - new members can only join when inviteed by other users (true) / everyone can join (false)
		bool				_topicLocked;	//mode 't' - topic of the channel can (false) or cannot be changed (true)
		bool				_hasKey;		//mode 'k' - the channel has key (password) (true) or doesn't have it (false)
		std::string			_key;			//mode 'k''s key (password) storage
		unsigned int		_limit;			//mode 'l' - the channel doesn't have any limit of users (0) or has a limit of users within the range of unsigned int (1 - 4,294,967,295)
};

#endif
