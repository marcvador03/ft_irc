/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:44:16 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/19 19:35:02 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*Welcome Sequence*/
static	void welcomeSequence( Client &c )
{
	//1. RPL WELCOME
	Reply rpl_welcome(c);
	rpl_welcome.list(c.getNickname()); // client is not nickname, to be checked
	rpl_welcome.list("Welcome to the network, " + c.getNickname() + "!" + c.getUser() + "@" + c.getHost());
	rpl_welcome.ship(1);
	//2. RPL_YOURHOST
	Reply rpl_yourhost(c);
	rpl_yourhost.list(c.getNickname()); // client is not nickname, to be checked
	rpl_yourhost.list("Your host is ");
	rpl_yourhost.list(c.getServername());
	rpl_yourhost.ship(2);
	//3. RPL_CREATED
	Reply rpl_created(c);
	rpl_created.list(c.getNickname()); // client is not nickname, to be checked
	rpl_created.list("The server was created ");
	rpl_created.list(c.getServerLaunchTime());
	rpl_created.list(", running version ");
	rpl_created.list(c.getServerVersion());
	rpl_created.ship(3);
	//4. RPL_MYINFO
	Reply rpl_myinfo(c);
	rpl_myinfo.list(c.getNickname()); // client is not nickname, to be checked
	rpl_myinfo.list(c.getServername());
	rpl_myinfo.list(c.getServerVersion());
	rpl_myinfo.list("o");
	rpl_myinfo.list("itlbk");
	rpl_myinfo.ship(4);
	//5. RPL_ISUPORT
	t_settings	settings = c.getServerSettings();
	Reply rpl_isuport(c);
	for (size_t i = 0; i < settings.size(); i++)
	{
			rpl_isuport.list(c.getNickname()); // client is not nickname, to be checked
			for (t_list::const_iterator it = settings[i].begin(); it != settings[i].end(); it++)	
				rpl_isuport.list(it->first + "=" + it->second);
			rpl_isuport.list("are supported by this server");
			rpl_isuport.ship(5);
	}
	return;
}

/*NICK*/
void Client::handleNick( t_arg args ) 
{
	Reply	nick(*this);
	std::string oldNick = getNickname();
	
	switch (setNickname(args[1])) {
		case 431:
			nick.list("No Nickname provided");
			nick.ship(431);
			break;
		case 432:
			nick.ship(432);
			break;
		case 433:
			nick.list(args[1]);
			nick.list("Nickname already in use");
			nick.ship(433);
			break;
		case 1:
			nick.list(args[0]);
			nick.list(args[1]);
			nick.ship();
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
			welcomeSequence(*this);
			break;
		case 0:
			nick.list(args[0]);
			nick.list(args[1]);
			nick.ship();
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
	}
	return;
}

/*USER*/
void Client::handleUser( t_arg args ) 
{
	Reply	user(*this);

	switch (setUser(args[1], args[4])) {
		case 431:
			user.ship(431);
			break;
		case 1:
			std::cout << "Username and Realname changed" << std::endl;
			welcomeSequence(*this);
			break;
		case 0:
			std::cout << "Username and Realname changed" << std::endl;
	}
	return;
}

/*PASS*/
void Client::handlePass( t_arg args ) 
{
	Reply	pass(*this);
	
	switch (registerPass(args[1])) {
		case 461:
			pass.ship(461);
			break;
		case 462:
			pass.ship(462);
			break;
		case 464:
			pass.ship(464);
			break;
		case 0:
			std::cout << "Password accepted" << std::endl;
	}
	return;
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
	Reply	quit(*this);
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
	quit.list("ERROR");
	quit.list("Client has disconnected");
	//marc: then just call the reply method with the vector cmd_reply
	quit.ship();
	//c.reply("ERROR :Closing Link: " + quitMsg + "\r\n");
	
	/*marc: for now stuck... I don't know how to delete the Client since
	 * then we come back to the ReceiveInput function from that same client :D 
	 * Update: solved by moving ReceiveInput and LaunchCmd to Server Class..... */ 
	_server->removeClient(this);
	return;
}
