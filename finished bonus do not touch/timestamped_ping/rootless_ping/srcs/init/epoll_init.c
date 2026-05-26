#include "../../ft_ping.h"

void	epoll_init(t_data *data)
{
	struct epoll_event	ev;
	
	data->epoll_fd = epoll_create(1);
	if (data->epoll_fd == -1)
	{
		perror("failed epoll creation");
		free_data(data);
		exit(1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = data->raw_sock;
	epoll_ctl(data->epoll_fd, EPOLL_CTL_ADD, data->raw_sock, &ev);
}
