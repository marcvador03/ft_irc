/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:10:59 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 11:57:09 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP
# include <iostream>
# include <string>
# include <cerrno>
# include "Client.hpp"
# include <bits/stdc++.h>

class Client;
class Error {
	public:
		class ErrnoException: public std::exception {
			virtual const char* what() const throw(); 
		};

		class NoNickNameGiven: public std::exception {
			virtual const char* what(Client &c) const throw(); 
		};

		class ServerPortIncorrectRange: public std::exception {
			virtual const char* what() const throw();
		};
};
#endif
