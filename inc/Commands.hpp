/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:38:50 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 10:49:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS_MARC_HPP
# define CMDS_MARC_HPP
# include <map>
# include <string>

class Client;
typedef void (*cmdPtr)(Client &);
typedef std::map<std::string, cmdPtr> t_list;
static t_list	cmdList;

void init_command_list();
void handlePing( Client &c ); 

#endif

