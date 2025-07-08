/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_by_Chatgpt_4_1.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:17:51 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/07 19:11:13 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name)
    : _name(name), _inviteOnly(false), _topicLocked(false), _hasKey(false), _limit(0) {}

Channel::~Channel() {}

const std::string& Channel::getName() const { return _name; }
const std::string& Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }

void Channel::setInviteOnly(bool inviteOnly) { _inviteOnly = inviteOnly; }
bool Channel::isInviteOnly() const { return _inviteOnly; }

void Channel::setTopicLocked(bool locked) { _topicLocked = locked; }
bool Channel::isTopicLocked() const { return _topicLocked; }

void Channel::setKey(const std::string& key) { _key = key; _hasKey = true; }
bool Channel::checkKey(const std::string& key) const { return _hasKey && _key == key; }

void Channel::setLimit(int limit) { _limit = limit; }
int Channel::getLimit() const { return _limit; }

bool Channel::addMember(Client* client) {
    if (_limit > 0 && (int)_members.size() >= _limit) return false;
    return _members.insert(client).second;
}
bool Channel::removeMember(Client* client) { return _members.erase(client) > 0; }
bool Channel::isMember(Client* client) const { return _members.count(client) > 0; }

bool Channel::addOperator(Client* client) { return _operators.insert(client).second; }
bool Channel::removeOperator(Client* client) { return _operators.erase(client) > 0; }
bool Channel::isOperator(Client* client) const { return _operators.count(client) > 0; }

void Channel::broadcast(const std::string& message) {
    for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        // (*it)->sendMessage(message); // Implement sendMessage in Client
    }
}
