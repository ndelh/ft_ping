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
	struct addrinfo	hints;
	int		error;
	
	setup_hints(&hints);
	error = getaddrinfo(data->hostname, NULL, &hints, &data->target_intel);
	if (error)
	{
		dprintf(2, "Ping: Error while retrieving hostname intel%s\n", gai_strerror(error));
		exit(1);
	}
}
