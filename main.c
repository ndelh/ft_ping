#include "ft_ping.h"

void	prerequisite_check(void)
{
	if (getuid())
	{
		write(2, "user must be root\n", 18);
		exit(0);
	}
}
t_data	*init_data(void)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data)
		memset(data, 0, sizeof(t_data));
	else
		perror("malloc failed");
	access_data(data);
	return (data);
}

int	main(int ac, char **argv)
{
	t_data	*data;
	
	if (ac < 2)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	prerequisite_check();
	data = init_data();
	if (!data)
		return (1);
	parse_retrieve_args(data, argv + 1);
	retrieve_target_intels(data);
	sig_init(data);
	create_socket(data);
	alarm(1);
	while (1)
	{
	};
	ft_end(data, 0);
}
