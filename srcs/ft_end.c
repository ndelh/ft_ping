#include "../ft_ping.h"

void	clean_data(t_data *data)
{
	if (data->raw_sock != -1)
		close(data->raw_sock);
	free(data->hostname);
	freeaddrinfo(data->target_intel);
	free(data);
}

void	ft_end(t_data *data, int exit_code)
{
	if (data)
		clean_data(data);
	exit(exit_code);
}
