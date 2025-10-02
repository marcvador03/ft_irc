/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:14:20 by mpietrza          #+#    #+#             */
/*   Updated: 2025/10/02 13:34:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include "ft_irc.h"

/*template<typename T>
void	log(T input)
{
	std::cout << "\033[0;36m" << input << "\033[0;37m "<< std::endl; 
};*/

template<typename U> 
std::vector<U>	split(const U &input, char delimiter)
{
	std::vector<U>		tokens;
	std::string 		token;
	std::string 		s(input);
	std::istringstream	tokenStream(s);
	
	while (std::getline(tokenStream, token, delimiter)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
};

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
