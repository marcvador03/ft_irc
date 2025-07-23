/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:49:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/23 23:09:54 by mfleury          ###   ########.fr       */
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

/** Command: JOIN
 * @brief The JOIN command is used to join a channel.
 * If the channel does not exist, it will be created.
 * <key> is an optional parameter that can be used to join a channel with a password.
 * @param <channel>{,<channel>} [<key>]
 * @return Void;
 */
void handleJoin( Client &c ) 
{
	
	t_cmd_reply			cmd_reply;
	t_list				list;
	t_list::iterator	it;
	std::string			chan, key;
	
	std::istringstream chan_s(c.args[1]);
	std::istringstream key_s(c.args[2]);
	
	//special case: "JOIN 0" means leave all channels
	if (c.args[1] == "0")
	{
		c.leaveAllChannels();	
		return ;
	}
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply(461);
		//c.replyMsg("461 JOIN :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
	{
		std::getline(key_s, key, ',');
		list.insert(std::make_pair(chan, key));
	}
	for (it = list.begin(); it!= list.end(); it++)
	{
		cmd_reply.push_back(c.args[0]);
		cmd_reply.push_back(it->first);
		switch (c.joinChannel(it->first, it->second)) {
			case 405:
				c.reply(405);
				//c.reply(": You have joined too many channels");
				break ;
			case 475:
				c.reply(475);
				//c.reply(": Cannot join channel (+k)");
				break ;
			case 471:
				c.reply(471);
				//c.reply(": Cannot join channel (+l)");
				break ;
			case 473:
				c.reply(473);
				//c.reply(": Cannot join channel (+i)");
				break ;
			case 476:
				c.reply(476);
				//c.reply(": Bad Channel Mask");
				break ;
			case 0:
				//c.reply(":" + c.getNickname() + " JOIN " + it->first);
				c.reply(cmd_reply);
				//c.reply(c.getNickname() + " joined " + it->first);
		}
	}
}

void handlePart( Client &c ) 
{
	t_cmd_reply			cmd_reply;
	t_list				list;
	t_list::iterator 	it;
	std::istringstream	chan_s(c.args[1]);
	std::string			chan;
	
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply(461);
		//c.reply("461 PART :Not enough parameters"); // ERR_NEEDMOREPARAMS
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
		list.insert(std::make_pair(chan, c.args[2]));
	for (it = list.begin(); it!= list.end(); it++)
	{
		switch (c.leaveChannel(it->first)) {
			case 403:
				c.reply(403);
				//c.reply(": No such channel");
				break ;
			case 442:
				c.reply(442);
				//c.reply(": You're not on that channel");
				break ;
			case 0:
				cmd_reply.push_back(c.args[0]);
				cmd_reply.push_back(it->first);
				c.reply(cmd_reply);
				//c.reply(":" + c.getNickname() + " PART " + it->first);
		}
	}
}

void handlePing( Client &c ) 
{
	t_cmd_reply			cmd_reply;
	
	//check if the token is present and non-empty
	if (c.args.size() < 2 || c.args[1].empty()) 
	{
		c.reply(409);
		return ;
	}
	cmd_reply.push_back("PONG");
	cmd_reply.push_back(c.args[1]);
	c.reply(cmd_reply);
	std::cout << "PONG sent with token: " << c.args[1] << std::endl;
}

static	void welcomeSequence( Client &c )
{
	t_cmd_reply			cmd_reply;

	//several parameters to implement

	//1. RPL WELCOME
	cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
	cmd_reply.push_back("Welcome to the network, " + c.getNickname() + "!" + c.getUser() + "@" + c.getHost());
	c.reply(1, cmd_reply);
	cmd_reply.clear();
	//2. RPL_YOURHOST
	cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
	cmd_reply.push_back("Your host is ");
	cmd_reply.push_back(c.getServername());
	c.reply(2, cmd_reply);
	cmd_reply.clear();
	//3. RPL_CREATED
	cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
	cmd_reply.push_back("The server was created ");
	cmd_reply.push_back(c.getServerLaunchTime());
	cmd_reply.push_back(", running version ");
	cmd_reply.push_back(c.getServerVersion());
	c.reply(3, cmd_reply);
	cmd_reply.clear();
	//4. RPL_MYINFO
	cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
	cmd_reply.push_back(c.getServername());
	cmd_reply.push_back(c.getServerVersion());
	cmd_reply.push_back("oi");
	cmd_reply.push_back("bkl");
	c.reply(4, cmd_reply);
	cmd_reply.clear();
	//5. RPL_ISUPORT
	t_set	settings = c.getServerSettings();
	for (size_t i = 0; i < settings.size(); i++)
	{
			cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
			for (size_t j = 0; j < settings[i].size(); j++)
				cmd_reply.push_back(settings[i].at(j));
			cmd_reply.push_back("are supported by this server");
			c.reply(5, cmd_reply);
			cmd_reply.clear();
	}
	return;
}

void handleNick( Client &c ) 
{
	t_cmd_reply			cmd_reply;
	std::string oldNick = c.getNickname();
	
	switch (c.setNickname(c.args[1])) {
		case 431:
			cmd_reply.push_back("No Nickname provided");
			c.reply(431, cmd_reply);
			//c.reply("431 NICK :No nickname given\r\n");
			break;
		case 432:
			c.reply(432);
			//c.reply("432 NICK :Erroneous nickname\r\n");
			break;
		case 433:
			cmd_reply.push_back(c.args[1]);
			cmd_reply.push_back("Nickname already in use");
			c.reply(433, cmd_reply);
			//c.reply("433 NICK :Nickname is already in use\r\n");
			std::cout << "Nickname '" << c.args[1] << "'is already in use." << std::endl;
			break;
		case 1:
			cmd_reply.push_back(c.args[0]);
			cmd_reply.push_back(c.args[1]);
			c.reply(c.args[1], cmd_reply);
			std::cout << "Nickname changed from '" << oldNick << "' to '" << c.args[1] << "'." << std::endl;
			welcomeSequence(c);
			break;
		case 0:
			cmd_reply.push_back(c.args[0]);
			cmd_reply.push_back(c.args[1]);
			c.reply(c.args[1], cmd_reply);
			std::cout << "Nickname changed from '" << oldNick << "' to '" << c.args[1] << "'." << std::endl;
	}
}

void handleUser( Client &c ) 
{
	t_cmd_reply			cmd_reply;
	
	switch (c.setUser(c.args[1], c.args[4])) {
		case 431:
			c.reply(431);
			break;
		case 1:
			std::cout << "Username and Realname changed" << std::endl;
			welcomeSequence(c);
			break;
		case 0:
			std::cout << "Username and Realname changed" << std::endl;
	}
}

void handlePass( Client &c ) 
{
	t_cmd_reply			cmd_reply;
	
	switch (c.registerPass(c.args[1])) {
		case 461:
			c.reply(461);
			break;
		case 462:
			c.reply(462);
			break;
		case 464:
			c.reply(464);
			break;
		case 0:
			std::cout << "Password accepted" << std::endl;
	}
}
/**
>>>>>>> 17c17313d81fd0254ccaff77446c2179fc7b70f8

### **Summary of steps in code for handleQuit**

1. Parse the reason (if any) from the QUIT command.
2. Notify all other clients in the same channels with the formatted QUIT message.
3. Optionally send an ERROR message to the quitting client.
4. Remove the client from all channels and close the connection.

---

**Example flow:**

- Client sends: `QUIT :Bye for now!`
- Server sends to other clients: `:dan-!d@localhost QUIT :Quit: Bye for now!`
- Server sends to quitting client: `ERROR :Closing Link: Quit: Bye for now!`
- Server closes the connection and removes the client.

--- */
void handleQuit(Server &s, Client &c)
{
	std::string reason = (c.args.size() > 1) ? c.args[1] : "";
	std::string quitMsg = ":Quit " + reason;
	std::string prefix = ":" + c.getNickname() + "!" + c.getUser() + "@localhost QUIT " + quitMsg + "\r\n";
	std::vector<Channel *> channels = s.getChannelsForClient(&c);
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		(*it)->broadcast(prefix, &c);
	c.reply("ERROR :Closing Link: " + quitMsg + "\r\n");
	s.removeClient(&c);
	close(c.getClientfd());
}
