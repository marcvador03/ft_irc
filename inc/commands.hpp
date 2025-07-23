/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:50:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/23 12:49:35 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include <string>
# include <iostream>
# include <cerrno>
# include <bits/stdc++.h>

class Client;
class Server;

void handlePass( Client &c );
void handleJoin( Client &c );
void handlePing( Client &c );
void handleNick( Client &c ); 
void handleQuit( Server &s, Client &c );

#endif

