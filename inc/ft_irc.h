/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:25:37 by mfleury           #+#    #+#             */
/*   Updated: 2025/09/25 14:19:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
# define FT_IRC_H
# include <iostream>
# include <string>
# include <csignal>
# include <iterator>
# include <cerrno>
# include <map>
# include <vector>
# include <deque>
# include "utils.hpp"

typedef std::vector<std::string> t_cmd_reply;
typedef std::map<int, std::string> t_arg;
typedef std::map<std::string, std::string> t_list;
typedef std::vector<t_list> t_settings;

#endif
