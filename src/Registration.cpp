/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:44:16 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/06 18:53:48 by mpietrza         ###   ########.fr       */
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
			err_ErroneusNickname(args[1]);
			break;
		case 433:
			err_NicknameInUse(args[1]);
			break;
		case 1:
			nick.list(args[0]);
			nick.list(args[1]);
			nick.ship();
			std::cout << "Nickname changed from '" << oldNick << "' to '" << args[1] << "'." << std::endl;
			rpl_Welcome();
			rpl_YourHost();
			rpl_Created();
			rpl_MyInfo();
			rpl_ISupport();
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
	}
	return;
}

/*PASS*/
void Client::handlePass( t_arg args ) 
{
	Reply	pass(*this);
	
	switch (registerPass(args[1])) {
		case 461:
			err_NeedMoreParameters(args[0]);
			break;
		case 462:
			err_AlreadyRegistered();
			break;
		case 464:
			err_PasswdMismatch();
			break;
		case 1:
			std::cout << "No password set on server, connection accepted" << std::endl;
			break;
		case 0:
			std::cout << "Password accepted" << std::endl;
	}
	return;
}

void Client::handleQuit( t_arg args ) 
{
	Reply	quit(*this, _nickname, 's', 'n');
	std::string reason = (args.size() > 1) ? args[1] : "";
	
	leaveAllChannels();
	quit.list("QUIT");
	quit.list("Quit: " + args[1]);
	quit.ship();
	_server->removeClient(this);
	return;
}
