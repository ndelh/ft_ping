#include "../ft_ping.h"

void	create_socket(t_data *data)
{

	data->raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->raw_sock == -1)
		perror("error encoutered while creating raw socket");
}
