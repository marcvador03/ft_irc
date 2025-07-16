/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:22:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/16 11:45:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Errors.hpp"

const char* Error::ErrnoException::what() const throw() 
{
	return std::strerror(errno);
};

const char* Error::NoNickNameGiven::what(Client &c) const throw() 
{
	std::string str;
	str = c.getClientfd() + ": No nickname given";
	
	return   str.c_str();
};
const char* Error::ServerPortIncorrectRange::what() const throw() 
{
	return "Port xx cannot be used for IRC server"; 
}; 
