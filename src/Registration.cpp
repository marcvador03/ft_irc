/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:44:16 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/21 20:02:59 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

/*NICK*/
void Client::handleNick( t_arg args ) 
{
	Reply	nick(*this);
	std::string oldNick = getNickname();
	
	switch (setNickname(args[1])) {
		case 431:
			err_NoNicknameGiven();
			break;
		case 432:
			err_ErroneousNickname(args[1]);
			break;
		case 433:
			err_NicknameInUse(args[1]);
			break;
		case 1: //first time registration of nickname on server
			rpl_Welcome();
			rpl_YourHost();
			rpl_Created();
			rpl_MyInfo();
			rpl_ISupport();
			break;
		case 0: //nickname change after registration
			nick.list(args[0]);
			nick.list(args[1]);
			nick.ship();
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
			break;
	}
	return;
}

/*USER*/
void Client::handleUser( t_arg args ) 
{
	Reply	user(*this);

	switch (setUser(args[1], args[4])) {
		case 431:
			err_NoNicknameGiven();
			break;
		case 1:
			std::cout << "Username and Realname changed" << std::endl;
			rpl_Welcome();
			rpl_YourHost();
			rpl_Created();
			rpl_MyInfo();
			rpl_ISupport();
			break;
		case 0:
			std::cout << "Username and Realname changed" << std::endl;
			break;
	}
	return;
}

/*PASS*/
void Client::handlePass(t_arg args)
{
	// Expected layout: args[0] = "PASS", args[1] = <password>

	// Already registered? (RFC: PASS must be sent before registration completes)
	if (isRegistered())
	{
		err_AlreadyRegistered();
		return;
	}

	// Not enough parameters (need the password token)
	if (args.size() < 2 || args[1].empty())
	{
		err_NeedMoreParameters("PASS");
		return;
	}

	// Now we have a candidate password
	switch (registerPass(args[1]))
	{
		case 461: // 2nd check
			err_NeedMoreParameters("PASS");
			break;
		case 462:
			err_AlreadyRegistered();
			break;
		case 464:
			err_PasswdMismatch();
			break;
		case 0:
			std::cout << "Password accepted" << std::endl;
			break;
		default:
			std::cerr << "registerPass() returned unexpected code\n";
			break;
	}
}

/*QUIT*/
void Client::handleQuit( t_arg args ) 
{
	Reply	quit(*this, _nickname, 's', 'n');
	std::string reason;
	if (args.size() > 1)
		reason = args[1];
	//if more reasons print all
	for (size_t i = 2; i < args.size(); ++i) {
		if (!reason.empty())
			reason += " ";
		reason += args[i];
	}

	//strip leading ':'
	if (!reason.empty() && reason[0] == ':')
		reason.erase(0, 1);
	
	if (reason.empty())
		reason = "Client Quit";

	leaveAllChannels();
	quit.list("QUIT");
	quit.list(":" + reason);
	quit.ship();
	_server->removeClient(this);
}
