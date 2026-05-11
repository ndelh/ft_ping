#include "ft_ping.h"

int	main(int ac, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (ac < 2)
		ft_putendl_fd("Ping: usage error: destination address required", 2);
	init(&data, argv + 1);
}
