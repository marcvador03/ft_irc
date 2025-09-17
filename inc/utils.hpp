/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:14:20 by mpietrza          #+#    #+#             */
/*   Updated: 2025/09/17 14:13:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>

std::string trim(const std::string &str);
std::vector<std::string> split(const std::string &s, char delimiter);

template<typename T>
void	log(T);
#include "utils.tpp"

#endif
