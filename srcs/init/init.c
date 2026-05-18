#include "../../ft_ping.h"

void	pre_check(void)
{
	if (getuid() != 0)
	{
		ft_putendl_fd("ft_ping: too few right, this program must be run as root", 2);
		exit(1);
	}	
}

void	error_flags(char *s)
{ 
	ft_putstr_fd("ping: Error: unsupported flags: -", 2);
	write(2, s, 1);
	write(2, "\n", 1);
}

void	parse_flags(t_data *data, char *s)
{
	while (*s)
	{
		if (*s == '?')
			data->flags |= FLAG_QM;
		else if (*s == 'v')
			data->flags |= FLAG_VLOWER;
		else
			error_flags(s);
		++s;
	}
}

void	retrieve_args_flags(t_data *data, char **s)
{
	while (*s)
	{
		if (**s == '-')
		       parse_flags(data, *s + 1);
		else
			data->hostname = *s;
		++s;
	}
}

void	init(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->sequence_tab = calloc(UINT16_MAX, sizeof(t_tab));
	if (!data->sequence_tab)
	{
		perror("alloc failed");
		exit(1);
	}
	data->raw_sock = -1;
	data->epoll_fd = -1;
	data->be_pid = htons(getpid());
	pre_check();
	retrieve_args_flags(data, argv);
	get_addr(data);
	open_raw_socket(data);
	epoll_init(data);
	sig_init(data);
}
