#include "../../ft_ping.h"

void	close_fds(t_data *data)
{
	if (data->raw_sock != -1)
		close(data->raw_sock);
	if (data->epoll_fd != -1)
		close(data->epoll_fd);
	if (data->timerfd != -1)
		close(data->timerfd);
}

void	destroy_alloc(t_data *data)
{
	(void)data;
}

void	free_data(t_data *data)
{
	close_fds(data);
	destroy_alloc(data);
}
