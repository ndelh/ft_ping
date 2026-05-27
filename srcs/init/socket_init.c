/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 11:20:37 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:23:24 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

void	setup_hints(struct addrinfo *hints)
{
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_protocol = IPPROTO_ICMP;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_flags = AI_CANONNAME;
}

void	get_name(t_data *data)
{
	socklen_t			size;
	struct sockaddr_in	*socka;

	socka = (struct sockaddr_in *)(&data->target_intel);
	size = sizeof(struct sockaddr_in);
	inet_ntop(AF_INET, &(socka->sin_addr), data->printable_ip, INET_ADDRSTRLEN);
	getnameinfo(&data->target_intel, size,
		data->true_name, NI_MAXHOST, NULL, 0, 0);
}

void	get_addr(t_data *data)
{
	struct addrinfo	*res;
	struct addrinfo	hints;
	int				error;

	if (data->flags & FLAG_VLOWER)
		printf("ping: sock.4fd: %i (socktype: SOCK_RAW), "
			"hints.ai_family: AF_INET\n\n", data->raw_sock);
	setup_hints(&hints);
	error = getaddrinfo(data->hostname, NULL, &hints, &res);
	if (error)
	{
		dprintf(2, "Ping: Error while retrieving hostname intel: %s\n",
			gai_strerror(error));
		free_data(data);
		exit(1);
	}
	memcpy(&data->target_intel, res->ai_addr, sizeof(struct sockaddr));
	get_name(data);
	if (data->flags & FLAG_VLOWER)
		printf("ai->ai_family: AF_INET, ai->ai_canonname: '%s'\n",
			res->ai_canonname);
	freeaddrinfo(res);
}

void	set_ttl(t_data *data)
{
	if (setsockopt(data->raw_sock, IPPROTO_IP, IP_TTL,
			&data->current_ttl, sizeof(uint8_t)))
	{
		perror("failed to set sockoption");
		free_data(data);
		exit(1);
	}
}

void	open_raw_socket(t_data *data)
{
	data->raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->raw_sock == -1)
	{
		perror("failed with data raw_sock");
		free_data(data);
		exit(1);
	}
	if (data->current_ttl)
		set_ttl(data);
}
