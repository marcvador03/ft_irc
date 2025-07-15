/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:08:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/15 13:25:32 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
# define PING_HPP
# include <iostream>
# include <string>
# include <cerrno>
# include <map>
# include <sys/socket.h>
# include "ACmd.hpp"

class Ping: public ACmd {
	public:
		Ping ( t_arg args );
		~Ping ( void );

		void 	run ( Client &c );

	private:
		Ping ( const Ping &other );
		Ping &operator=( const Ping &other );
};

#endif
