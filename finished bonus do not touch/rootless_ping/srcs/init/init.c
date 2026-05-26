#include "../../ft_ping.h"

void	pre_check(void)
{
	if (getuid() != 0)
	{
		ft_putendl_fd("ft_ping: too few right, this program must be run as root", 2);
		exit(1);
	}	
}

void	init(t_data *data, char **argv, int ac)
{
	memset(data, 0, sizeof(t_data));
	data->sequence_tab = calloc(UINT16_MAX + 1, sizeof(t_tab));
	if (!data->sequence_tab)
	{
		perror("alloc failed");
		exit(1);
	}
	data->raw_sock = -1;
	data->epoll_fd = -1;
	data->be_pid = htons(getpid());
	//pre_check();
	retrieve_args(data, argv, ac);
	open_raw_socket(data);
	get_addr(data);
	epoll_init(data);
	sig_init(data);
	data->min_time = UINT64_MAX;
}
