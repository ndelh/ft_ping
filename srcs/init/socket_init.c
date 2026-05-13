#include "../../ft_ping.h"

void	setup_hints(struct addrinfo *hints)
{
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_protocol = IPPROTO_ICMP;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_flags = AI_CANONNAME;
}

void	get_addr(t_data *data)
{
	struct addrinfo *res;
	struct addrinfo	hints;
	int		error;
	
	setup_hints(&hints);
	error = getaddrinfo(data->hostname, NULL, &hints, &res);
	if (error)
	{
		dprintf(2, "Ping: Error while retrieving hostname intel: %s\n", gai_strerror(error));
		exit(1);
	}
	memcpy(&data->target_intel, res->ai_addr, sizeof(struct sockaddr));
	freeaddrinfo(res);
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
}
