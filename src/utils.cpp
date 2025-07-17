/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:18:27 by mpietrza          #+#    #+#             */
/*   Updated: 2025/07/16 12:55:29 by mpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/utils.hpp"
#include "../inc/Client.hpp"
#include <sstream>

std::string trim(const std::string &str) {
	const std::string WHITESPACE = " \n\r\t\f\v";
	size_t start = str.find_first_not_of(WHITESPACE);
	if (start == std::string::npos)
		//find_first_not_of() - C++ `std::string` class; this function is used to search a string (`str`) for the first character that does **not** match any of the characters specified in its argument
		// It returns the index of that character, or `std::string::npos` if all characters match the set
		return "";
	size_t end = str.find_last_not_of(WHITESPACE);
	return str.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string &s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);;
	while (std::getline(tokenStream, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}
