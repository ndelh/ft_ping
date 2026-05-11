#include "../ft_ping.h"

void	setup_hints(struct addrinfo *hints)
{
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
	hints->ai_flags = AI_CANONNAME;
}

void	retrieve_target_intels(t_data *data)
{
	struct addrinfo hints;
	int	result;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	setup_hints(&hints);
	result = getaddrinfo(data->hostname, NULL, &hints, &data->target_intel);
	if (result)
	{
		dprintf(2, "error while retrieving hostname intel: %s\n", gai_strerror(result));
		ft_end(data, 2);
	}
	
}

void	load_header(t_data *data)
{
	struct icmphdr	*header;

	header = data->header;
	header->type = ICMP_ECHO;
	header->code = 0;
	header->un.echo.id = data->pid;
	header->un.echo.sequence = 1;
}

void	create_socket(t_data *data)
{
	data->raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->raw_sock == -1)
		perror("error encoutered while creating raw socket");
}
