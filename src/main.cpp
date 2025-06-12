/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/12 17:43:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_irc.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#define PORT 6667

int	main ( void )
{
	int					clientfd;
	ssize_t				bytes_received;
	char				buffer[1024];

	/*std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	socklen = sizeof(server_addr);
	bind(serverfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(serverfd, 5);*/
	Server server;
	clientfd = accept(serverfd, (struct sockaddr *)&server_addr, &socklen);
	bytes_received = read(clientfd, buffer, sizeof(buffer) - 1);
	buffer[bytes_received] = '\0';


	return (0);
}
