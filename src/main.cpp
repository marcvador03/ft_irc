/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpietrza <mpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/07/03 17:11:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.h"
#define PORT 6667

int	main ( void )
{
	/*int					clientfd;
	ssize_t				bytes_received;
	char				buffer[1024];*/

	Server server (60);
	poll(server.fds, 1, server.getTimeOut());
	while (!server.connections.empty())
	{
		for (long unsigned int i = 0; i < server.connections.size(); i++)
		{
			if (server.fds[i].revents & POLLIN)
				server.connections[i]->ReceiveInput();
			else if (server.fds[i].revents & (POLLERR | POLLHUP))
				std::cout << "we should close client fd" << std::endl;
				//close Client fd??
		}
	}
	
	//clientfd = accept(serverfd, (struct sockaddr *)&server_addr, &socklen);
	//bytes_received = read(clientfd, buffer, sizeof(buffer) - 1);
	//buffer[bytes_received] = '\0';


	return (0);
}
