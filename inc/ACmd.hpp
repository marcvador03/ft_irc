/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACmd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 12:58:11 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/15 13:45:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACMD_HPP
# define ACMD_HPP
# include <iostream>
# include <string>
# include <cerrno>
# include <map>
# include "Client.hpp"

class Client;

class ACmd {
	public:
		ACmd ( t_arg args );
		~ACmd ( void );

		virtual void 	run ( Client &c );

	protected:
		t_arg _args;

	private:
		ACmd ( const ACmd &other );
		ACmd &operator=( const ACmd &other );
};

#endif
