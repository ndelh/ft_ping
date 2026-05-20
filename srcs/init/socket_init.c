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

void	setup_sock_kernel_option(int *flags)
{
	*flags = SOF_TIMESTAMPING_RX_SOFTWARE
		| SOF_TIMESTAMPING_TX_SOFTWARE
		| SOF_TIMESTAMPING_SOFTWARE
		| SOF_TIMESTAMPING_OPT_CMSG;
}

void	open_raw_socket(t_data *data)
{
	int flags;
	int	recver;

	flags = 0;
	recver = 1;
	data->raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->raw_sock == -1)
	{
		perror("failed with data raw_sock");
		free_data(data);
		exit(1);
	}
	setup_sock_kernel_option(&flags);
	if (setsockopt(data->raw_sock, SOL_SOCKET, SO_TIMESTAMPING, &flags, sizeof(flags))
			|| setsockopt(data->raw_sock, SOL_IP, IP_RECVERR, &recver, sizeof(recver)))
	{
		perror("failed to set sock params");
		free_data(data);
		exit(1);
	}
}
