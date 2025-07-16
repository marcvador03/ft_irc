/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 09:50:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 15:04:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_MARC_HPP
# define CMDS_MARC_HPP
# include <string>
# include <iostream>
# include <cerrno>
# include <bits/stdc++.h>

class Client;

void handlePing( Client &c );
void handleNick( Client &c ); 
#endif

