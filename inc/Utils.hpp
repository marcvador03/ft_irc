/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:14:20 by mpietrza          #+#    #+#             */
/*   Updated: 2025/10/22 14:25:41 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include <cctype>
# include "ft_irc.h"

enum CaseMapping { CASEMAPPING_ASCII, CASEMAPPING_RFC1459 };

inline CaseMapping parseCaseMapping(const std::string &s)
{
	return (s == "ascii") ? CASEMAPPING_ASCII : CASEMAPPING_RFC1459;
}

inline char fold_ascii_char(unsigned char c)
{
	if (c >= 'A' && c <= 'Z')
		return static_cast<char>(c + ('a' - 'A'));
	return static_cast<char>(c);
}

inline char fold_rfc1459_char(unsigned char c)
{
	if (c >= 'A' && c <= 'Z')
		return static_cast<char>(c + ('a' - 'A'));
	if (c == '{')
		return '[';
	if (c == '}')
		return ']';
	if (c == '|')
		return '\\';

	return static_cast<char>(c);
}

// Template to fold any basic string-like type according to CaseMapping
template<typename StringT>
StringT casefold(const StringT &s, CaseMapping cm)
{
	StringT out;
	for (size_t i = 0; i < s.size(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(s[i]);
		out.push_back(cm == CASEMAPPING_ASCII ? fold_ascii_char(c) : fold_rfc1459_char(c));
	}
	return out;
}

template<typename T> 
std::vector<T>	split(const T &input, char delimiter)
{
	std::vector<T>		tokens;
	std::string 		token;
	std::string 		s(input);
	std::istringstream	tokenStream(s);
	
	while (std::getline(tokenStream, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
};

template<typename T> 
void toupper(T &str)
{
	for (size_t i = 0; i < str.size(); ++i)
		str[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));
}

template<typename T> 
std::string trimstr (const T &str) 
{
	const std::string WHITESPACE = " \n\r\t\f\v";
	size_t start = str.find_first_not_of(WHITESPACE);
	if (start == std::string::npos)
		return "";
	size_t end = str.find_last_not_of(WHITESPACE);
	return str.substr(start, end - start + 1);
}


/*template<typename U> 
std::vector<std::string> split(U input, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(input);
	
	while (std::getline(tokenStream, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
};*/
std::string trim(const std::string &str);

#endif
