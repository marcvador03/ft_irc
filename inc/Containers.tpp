/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Containers.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 12:35:38 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/24 12:37:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINERS_HPP
# define CONTAINERS_HPP

//`t_arg` is a type alias for a map that associates an integer (slot) with
//a string (argument). This is used to store command arguments for the client.
typedef std::vector<std::string> t_cmd_reply;
typedef std::map<int, std::string> t_arg;
typedef std::map<std::string, std::string> t_list;
typedef std::vector<std::vector<std::string> > t_set;

#endif
