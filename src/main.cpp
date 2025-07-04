/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/04 15:15:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.h"

int	main ( void )
{
	Server server (60);
	while (1)
	{
		poll(server.pfd, 200, -1);
		for (long unsigned int i = 0; i <= server.connections.size(); i++) // monitors all connected clients sockets for their commands
		{
			if (server.pfd[i].revents & POLLIN)
			{
				if (i == 0)
					server.addClient();
				else
				{	
					char	buf[256];
					int		bytes = recv(server.pfd[i].fd, buf, sizeof(buf), 0);
					if (bytes == 0)
						std::cout << "Connection lost" << std::endl;
					server.connections[i]->ReceiveInput();
				}
			}
		}
	}
	return (0);
}
