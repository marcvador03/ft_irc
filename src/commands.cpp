/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:49:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/18 14:33:13 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/commands.hpp"
#include "../inc/utils.hpp"

/*Based on the **ft_irc project requirements** and your code, you need to implement the following IRC commands:

---

### **Required Commands for ft_irc**

#### **Registration & Authentication**
- `PASS` — Set password for authentication.
- `NICK` — Set or change nickname.
- `USER` — Set username and real name.

#### **Channel Management**
- `JOIN` — Join/create a channel (already started).
- `PART` — Leave a channel.
- `TOPIC` — View or change the channel topic.
- `MODE` — Change channel modes (`i`, `t`, `k`, `o`, `l`).
- `KICK` — Eject a client from the channel.
- `INVITE` — Invite a client to a channel.

#### **Messaging**
- `PRIVMSG` — Send private messages to users or channels.

#### **Other**
- `PING` — Respond to keepalive checks (already started).
- `QUIT` — Disconnect from the server.
- `CAP` — (optional, for capability negotiation).

---

### **Summary Table**

| Command   | Purpose/Required?           |
|-----------|----------------------------|
| PASS      | Authentication             |
| NICK      | Nickname registration      |
| USER      | Username registration      |
| JOIN      | Channel join/create        |
| PART      | Channel leave              |
| TOPIC     | Channel topic              |
| MODE      | Channel modes              |
| KICK      | Operator: kick user        |
| INVITE    | Operator: invite user      |
| PRIVMSG   | Messaging                  |
| PING      | Keepalive                  |
| QUIT      | Disconnect                 |
| CAP       | Optional                   |

---

**You must implement:**  
- All commands above except `CAP` (unless your client requires it).

**You must also:**  
- Forward channel messages to all members.
- Support operators and regular users.
- Handle channel operator commands and modes.

*/

/** Command: PASS
 * @brief The PASS command is used to set the password for the connection.
 * If set, the password must be set before any attempt to register the
 * connection is made. This requires that clients send a PASS command before
 * sending the NICK / USER combination.
 * @param <password>
 * @return Void;
 */
void handlePass( Client &c ) 
{
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply("461 PASS :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
		return ;
	}
	/* TODO: the rest */
	/* need changes in Server class to store the password : 
		class Server {
	public:
		Server(const std::string& password);
		const std::string& getPassword() const;
		// ... other server methods ...
	private:
		std::string _password;
		// ... other members ...
	}; */
	/*
	// Check if the client is already registered
    if (c.isRegistered()) {
        c.reply("462 PASS :You may not reregister\r\n");
        return;
    }
	// Set the password for the client
    c.setPassword(c.args[1]);
    if (server.getPassword() != c.args[1]) {
        c.reply("464 PASS :Password incorrect\r\n");
        // Optionally close connection here
        return;
    }
	*/
}


//helper function for nickname validation
static bool isValidNickname(const std::string &nick)
{
	if (nick.empty())
		return false;
	if (nick[0] == '#' || nick[0] == ':' || std::isspace(nick[0]))
		return false;
	for (size_t i = 0; i < nick.size(); ++i)
	{
		char c = nick[i];
		if (!std::isalnum(c) && std::string("[]{}\\|").find(c) == std::string::npos)
			return false;
	}
	return true;
}

/* TODO: Client list in Server Class:
class Server {
public:
    // ... other methods ...
    bool isNicknameInUse(const std::string& nickname) const;
    std::vector<Client*> clients; // or std::map<int, Client*>
};

implementation of isNicknameInUse:
bool Server::isNicknameInUse(const std::string& nickname) const {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getNickname() == nickname)
            return true;
    }
    return false;
}
*/

/** Command: NICK
 * @brief The NICK command is used to give the client a nickname or change
 * the previous one.
 * The <nickname> parameter is the new nickname that the client wants to use.
 * If the nickname is already in use, the server will respond with an error.
 * @param <nickname>
 * @return Void;
 */
void handleNick( Server &s, Client &c )
{
	(void)s; // Unused parameter, but required by the function signature
	if (c.args.size() < 2 || c.args[1].empty())
	{
		c.reply("431 NICK :No nickname given\r\n");
		return ;
	}
	
	std::string newNick = c.args[1];

	if (!isValidNickname(newNick))
	{
		c.reply("432 NICK :Erroneous nickname\r\n");
		return ;
	}

/*	if (s.isNicknameInUse(NewNick))
	{
		c.reply("433 NICK :Nickname is already in use\r\n");
		std::cout << "Nickname '" << newNick << "' is already in use." << std::endl;
		return ;
	}*/
	
	std::string oldNick = c.getNickname();
	c.setNickname(newNick);

	c.reply(":" + oldNick + " NICK " + newNick + "\r\n");
	std::cout << "Nickname changed from '" << oldNick << "' to '" << newNick << "'." << std::endl;
}


/** Command: JOIN
 * @brief The JOIN command is used to join a channel.
 * If the channel does not exist, it will be created.
 * <key> is an optional parameter that can be used to join a channel with a password.
 * @param <channel>{,<channel>} [<key>]
 * @return Void;
 */
void handleJoin( Client &c ) 
{
	//check if channel argument is present
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply("461 JOIN :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
		return ;
	}

	//special case: "JOIN 0" means leave all channels
	if (c.args[1] == "0")
	{
		c.reply(":JOIN 0 not implemented yet\r\n");
		return ;
	}
//  the input looks like: "JOIN #foo,#bar,#baz key1,key2,key3"
// but at this point is already split into:
// args[0] = "JOIN"
// args[1] = "#foo,#bar,#baz" -channels
// args[2] = "key1,key2,key3" -keys

	//split channels and keys (if present)
	std::string channelStr = c.args[1];
	std::string keyStr = (c.args.size() > 2) ? c.args[2] : "";
	//if the user hasn't provided the key it will be changed to empty string
	
	std::vector<std::string> channels = split(channelStr, ',');
	std::vector<std::string> keys = split(keyStr, ',');

	for (std::size_t i = 0; i < channels.size(); ++i)
	{
		std::string channel = channels[i];
		std::string key = (i < keys.size()) ? keys[i] : "";

	// TODO: Channel management, key checking, etc.
	
	c.reply(":" + c.getNickname() + " JOIN " + channel + "\r\n");
	}
}

void handlePing( Client &c ) 
{
	//check if the token is present and non-empty
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply("409 PING :No originn specified\r\n"); //ERR_NOORIGIN
		return ;
	}
	std::string token = c.args[1];
	c.reply("PONG :" + token + "\r\n");
	std::cout << "PONG sent with token: " << token << std::endl;
}

void handleNick(Server &s, Client &c ) 
{
	//check if nickname is provided
	if (c.args.size() < 2 || c.args[1].empty())
	{
		c.reply("431 NICK :No nickname given\r\n");
		return ;
	}

	std::string newNick = c.args[1];

	//validate nickname format
	if (!isValidNickname(newNick))
	{
		c.reply("432 NICK :Erroneous nickname\r\n");
		return ;
	}

	/* TODO: need a new server function : "bool isNickcnameInUse( const std::string &newNick) const"*/
	//check if nickname is not already in use
	/*if (s.isNickcnameInUse(newNick))
	{
		c.reply("433 NICK :Nickname is already in use\r\n");
		std::cout << "Nickname '" << newNick << "'is already in use." << std::endl;
		return ;
	}*/

	//set the new nickname
	std::string oldNick = c.getNickname();
	c.setNickname (newNick);

	//acknowledge the client and print to server log
	c.reply(":" + oldNick + " NICK " + newNick + "\r\n");
	std::cout << "Nickname changed from '" << oldNick << "' to '" << newNick << "'." << std::endl;
}


