//// filepath: /home/mpietrza/projects/05f_ft_irc/git_ft_irc/inc/Channel.hpp
#pragma once

#include <string>
#include <set>
#include "Client.hpp"

class Channel
{
private:
    std::string              _name;
    std::string              _topic;
    std::set<Client*>        _members;      // All clients in channel
    std::set<Client*>        _operators;    // Channel operators
    bool                     _inviteOnly;    // Mode 'i'
    bool                     _topicLocked;   // Mode 't'
    bool                     _hasKey;        // Mode 'k'
    std::string              _key;
    int                      _limit;         // Mode 'l'
    
public:
    // Constructor / Destructor
    Channel(const std::string& name);
    ~Channel();
    
    // Basic getters/setters
    const std::string& getName() const;
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);

    // Channel mode-related methods
    void setInviteOnly(bool inviteOnly);
    bool isInviteOnly() const;
    
    void setTopicLocked(bool locked);
    bool isTopicLocked() const;
    
    void setKey(const std::string& key);
    bool checkKey(const std::string& key) const;

    void setLimit(int limit);
    int getLimit() const;

    // Membership and operator management
    bool addMember(Client* client);
    bool removeMember(Client* client);
    bool isMember(Client* client) const;

    bool addOperator(Client* client);
    bool removeOperator(Client* client);
    bool isOperator(Client* client) const;

    // Broadcast
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
	}  void broadcast(const std::string& message);
};