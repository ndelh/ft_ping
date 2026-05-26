#include "ft_ping.h"

t_data	*access_data(t_data *data)
{
	static t_data	*data_pointer;

	if (!data_pointer)
		data_pointer = data;
	return (data_pointer);
}

int	main(int ac, char **argv)
{
	t_data	data;

	if (ac < 2)
		ft_putendl_fd("Ping: usage error: destination address required", 2);
	init(&data, argv, ac);
	access_data(&data);
	core_loop(&data);
}
