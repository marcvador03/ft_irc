/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:49:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/10 13:28:36 by mfleury          ###   ########.fr       */
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

/** Command: JOIN
 * @brief The JOIN command is used to join a channel.
 * If the channel does not exist, it will be created.
 * <key> is an optional parameter that can be used to join a channel with a password.
 * @param <channel>{,<channel>} [<key>]
 * @return Void;
 */

void Client::handleJoin( t_arg args ) 
{
	
	t_cmd_reply			cmd_reply;
	t_list				list;
	t_list::iterator	it;
	std::string			chan, key;
	
	std::istringstream chan_s(args[1]);
	std::istringstream key_s(args[2]);
	
	//special case: "JOIN 0" means leave all channels
	if (args[1] == "0")
	{
		leaveAllChannels();	
		return ;
	}
	if (args.size() < 2 || args[1].empty()) 
	{
		reply(461);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
	{
		std::getline(key_s, key, ',');
		list.insert(std::make_pair(chan, key));
	}
	for (it = list.begin(); it!= list.end(); it++)
	{
		cmd_reply.push_back(args[0]);
		cmd_reply.push_back(it->first);
		switch (joinChannel(it->first, it->second)) {
			case 405:
				reply(405);
				break ;
			case 475:
				reply(475);
				break ;
			case 471:
				reply(471);
				break ;
			case 473:
				reply(473);
				break ;
			case 476:
				reply(476);
				break ;
			case 0:
				reply(cmd_reply);
		}
	}
}

void Client::handlePart( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	t_list				list;
	t_list::iterator 	it;
	std::istringstream	chan_s(args[1]);
	std::string			chan;
	
	if (args.size() < 2 || args[1].empty()) 
	{
		reply(461);
		return ;
	}
	for (int j = 0; std::getline(chan_s, chan, ','); j++)
		list.insert(std::make_pair(chan, args[2]));
	for (it = list.begin(); it!= list.end(); it++)
	{
		switch (leaveChannel(it->first)) {
			case 403:
				reply(403);
				break ;
			case 442:
				reply(442);
				break ;
			case 0:
				cmd_reply.push_back(args[0]);
				cmd_reply.push_back(it->first);
				reply(cmd_reply);
		}
	}
}

void Client::handlePing( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	
	//check if the token is present and non-empty
	if (args.size() < 2 || args[1].empty()) 
	{
		reply(409);
		return ;
	}
	cmd_reply.push_back("PONG");
	cmd_reply.push_back(args[1]);
	reply(cmd_reply);
	std::cout << "PONG sent with token: " << args[1] << std::endl;
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
	t_settings	settings = c.getServerSettings();
	for (size_t i = 0; i < settings.size(); i++)
	{
			cmd_reply.push_back(c.getNickname()); // client is not nickname, to be checked
			for (t_list::const_iterator it = settings[i].begin(); it != settings[i].end(); i++)	
				cmd_reply.push_back(it->first + it->second);
			cmd_reply.push_back("are supported by this server");
			c.reply(5, cmd_reply);
			cmd_reply.clear();
	}
	return;
}

void Client::handleNick( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	std::string oldNick = getNickname();
	
	switch (setNickname(args[1])) {
		case 431:
			cmd_reply.push_back("No Nickname provided");
			reply(431, cmd_reply);
			break;
		case 432:
			reply(432);
			break;
		case 433:
			cmd_reply.push_back(args[1]);
			cmd_reply.push_back("Nickname already in use");
			reply(433, cmd_reply);
			std::cout << "Nickname '" << args[1] << "'is already in use." << std::endl;
			break;
		case 1:
			cmd_reply.push_back(args[0]);
			cmd_reply.push_back(args[1]);
			reply(args[1], cmd_reply);
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
			welcomeSequence(*this);
			break;
		case 0:
			cmd_reply.push_back(args[0]);
			cmd_reply.push_back(args[1]);
			reply(args[1], cmd_reply);
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
	}
}

void Client::handleUser( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	
	switch (setUser(args[1], args[4])) {
		case 431:
			reply(431);
			break;
		case 1:
			std::cout << "Username and Realname changed" << std::endl;
			welcomeSequence(*this);
			break;
		case 0:
			std::cout << "Username and Realname changed" << std::endl;
	}
}

void Client::handlePass( t_arg args ) 
{
	t_cmd_reply			cmd_reply;
	
	switch (registerPass(args[1])) {
		case 461:
			reply(461);
			break;
		case 462:
			reply(462);
			break;
		case 464:
			reply(464);
			break;
		case 0:
			std::cout << "Password accepted" << std::endl;
	}
}

/**

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
//void handleQuit(Server &s, Client &c)
/*I'm trying to keep consistent passing only the Client class. 
 * However you can get server variables through c._server (which is the link of the Server Class into the Client
 * and even have getters from Client Class which fetch a specific server variable. */
void Client::handleQuit( t_arg args ) 
{
	t_cmd_reply			cmd_reply; // marc: basic vector to store each argument and parameter
	std::string reason = (args.size() > 1) ? args[1] : "";
	
	//marc: moving to new cmd_reply model
	/*std::string quitMsg = ":Quit " + reason;
	std::string prefix = ":" + c.getNickname() + "!" + c.getUser() + "@localhost QUIT " + quitMsg + "\r\n";*/

	/* marc: actually there is already container in Client Class :) containing 
	 * all Channels to which the Client has joined (and is maintained through
	 * leaveChannel / joinChannel existing methods. And there is already a method
	 * called leaveAllChannels which just removes client from all channels */ 
	
	/*std::vector<Channel *> channels = s.getChannelsForClient(&c);
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		(*it)->broadcast(prefix, &c);*/
	leaveAllChannels();
	//marc: put your arguments / parameters in your cmd_reply as below
	cmd_reply.push_back("ERROR");
	cmd_reply.push_back("Client has disconnected");
	//marc: then just call the reply method with the vector cmd_reply
	reply(cmd_reply);
	//c.reply("ERROR :Closing Link: " + quitMsg + "\r\n");
	
	/*marc: for now stuck... I don't know how to delete the Client since
	 * then we come back to the ReceiveInput function from that same client :D 
	 * Update: solved by moving ReceiveInput and LaunchCmd to Server Class..... */ 
	_server->removeClient(this);
}
