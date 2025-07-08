/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:13:42 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/08 18:25:07 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <set>
#include "Client.hpp"

/*
**`Channel.hpp` / `Channel.cpp`**  
- Store channel name, topic, operators, invited users, etc.  
- Manage channel modes (`i`, `t`, `k`, `o`, `l`).  
- Forward messages to all users in channel.  
- Implement commands like `KICK`, `INVITE`, `TOPIC`, `MODE`.  
 */
class Channel
{
	private:
		std::string			_name;			//channel name
		std::string			_topic;			//channel topic
		std::set<Client *>	_members;		//all clients in channel
		std::set<Client *>	_operators;		//channel operators (for mode 'o')
		bool				_inviteOnly;	//mode 'i' - new members can only join when inviteed by other users (true) / everyone can join (false)
		bool				_topicLocked;	//mode 't' - topic of the channel can (false) or cannot be changed (true)
		bool				_hasKey;		//mode 'k' - the channel has key (password) (true) or doesn't have it (false)
		std::string			_key;			//mode 'k''s key (password) storage
		unsigned int		_limit;			//mode 'l' - the channel doesn't have any limit of users (0) or has a limit of users within the range of unsigned int (1 - 4,294,967,295)

	public:
		//Constructor and destructor
		Channel(const std::string &name);
		~Channel();

		//basic getters
		const std::string &getName() const;
		
		//invite only related
		void setInviteOnly(bool isInviteOnly);
		bool isInviteOnly() const;

		//topic related
		const std::string &getTopic() const;
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
};

#endif
