/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:31:02 by mfleury           #+#    #+#             */
/*   Updated: 2025/10/02 14:19:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

/*461*/
void	Client::err_NeedMoreParameters( const std::string &cmd ) 
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(cmd);
	err.list("Not enough parameters");
	err.ship(461);
	return ;
}

/*403*/
void	Client::err_noSuchChannel( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("Not such channel");
	err.ship(403);
	return ;
}

void	Client::err_tooManyChannels( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("You have joined too many channels");
	err.ship(405);
	return ;
}

void	Client::err_BadChannelKey( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("Cannot join channel (+k)");
	err.ship(475);
	return ;
}

void	Client::err_ChannelIsFull( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("Cannot join channel (+l)");
	err.ship(471);
	return ;
}

void	Client::err_InviteOnlyChan( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("Cannot join channel (+i)");
	err.ship(473);
	return ;
}

void	Client::err_BadChanMask( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("Bad channel mask");
	err.ship(476);
	return ;
}

void	Client::err_notOnChannel( const std::string &chan ) 
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("You're not on that channel");
	err.ship(442);
	return ;
}

void	Client::err_noTextToSend( void )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list("No text to send");
	err.ship(412);
	return ;
}

void	Client::err_noSuchNick( void )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(_nickname);
	err.list("No such nick/channel");
	err.ship(401);
	return ;
}

/*501*/
void	Client::err_UModeUnknownFlag( void )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list("Unknown MODE flag");
	err.ship(501);
	return;
}

/*482*/
void	Client::err_ChanOPrivsNeeded( const std::string &chan )
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list("You're not channel operator");
	err.ship(482);
	return ;

}

/*696*/
void	Client::err_InvalidModeParam( Channel &chan, const std::string &key, const std::string &param, const std::string &msg )
{
	Reply	err(*this);
	
	err.list(chan.getName());
	err.list(key);
	err.list(param);
	err.list(msg);
	err.ship(696);
	return ;
}

/*409*/
void	Client::err_NoOrigin( void )
{
	Reply	err(*this);
	
	err.list("No origin specified");
	err.ship(409);
	return ;

}

/*431*/
void	Client::err_NoNicknameGiven( void )
{
	Reply	err(*this);

	err.list("No nickname given");
	err.ship(431);
	return;
}

/*432*/
void	Client::err_ErroneusNickname( const std::string &nick )
{
	Reply	err(*this);
	
	err.list(nick);
	err.list("Erroneus nickname");
	err.ship(432);
	return;
}

/*433*/
void	Client::err_NicknameInUse( const std::string &nick )
{
	Reply	err(*this);

	err.list(nick);
	err.list("Nickname already in use");
	err.ship(433);
	return;
}

/*462*/
void	Client::err_AlreadyRegistered ( void )
{
	Reply	err(*this);
	
	err.list("You may not reregister");
	err.ship(462);
	return;
}

/*464*/
void	Client::err_PasswdMismatch ( void )
{
	Reply	err(*this);
	
	err.list("Password incorrect");
	err.ship(464);

	return;
}

/*443*/
void	Client::err_UserOnChannel( const std::string &chan ) 
{
	Reply	err(*this);

	err.list(_nickname);
	err.list(chan);
	err.list(":is already on channel");
	err.ship(443);
	return ;
}
