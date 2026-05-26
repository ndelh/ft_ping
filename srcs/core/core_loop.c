#include "../../ft_ping.h"

void	add_timer_fd(t_data *data)
{
	struct epoll_event	ev;

	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = data->timerfd;
	epoll_ctl(data->epoll_fd, EPOLL_CTL_ADD, data->timerfd, &ev);
}

void	set_timer_fd(t_data *data)
{
	struct itimerspec	a;

	data->timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (data->timerfd == -1)
	{
		free_data(data);
		perror("failed to create timer fd");
		exit(1);
	}
	memset(&a, 0, sizeof(struct itimerspec));
	a.it_interval.tv_sec = 1;
	a.it_value.tv_sec = 1;
	if (timerfd_settime(data->timerfd, 0, &a, NULL) == -1)
	{
		free_data(data);
		perror("failed to set timer fd");
		exit(1);
	}
	add_timer_fd(data);
}

void	sec_routine(t_data *data)
{
	uint64_t	expired;
	ssize_t		nb_read;

	nb_read = read(data->timerfd, &expired, sizeof(uint64_t));
	if (nb_read != sizeof(uint64_t))
	{
		ft_putendl_fd("Error: bad value received from timerfd", 2);
		free_data(data);
		exit(1);
	}
	send_ping(data);
}

void	core_loop(t_data *data)
{
	struct epoll_event	surveil[16];
	struct epoll_event	cursor;
	int			event_nb;
	int			i;

	set_timer_fd(data);
	print_begin(data);
	gettimeofday(&data->launch_time, 0);
	send_ping(data);
	while (1)
	{
		i = 0;
		event_nb = epoll_wait(data->epoll_fd, surveil, 16, -1);
		while (i < event_nb)
		{
			cursor = surveil[i];
			if (cursor.data.fd == data->timerfd)
				sec_routine(data);
			else
			{ 
				if (cursor.events & EPOLLERR)
					fetch_in_error_queue(data);
				if (cursor.events & EPOLLIN)
					receive_pong(data);
			}
			++i;
		}
	}
}

void	core_loop_f(t_data *data)
{
	struct epoll_event	surveil[16];
	struct epoll_event	cursor;
	int			event_nb;
	int			i;

	print_begin(data);
	gettimeofday(&data->launch_time, 0);
	send_ping(data);
	usleep(10000);
	while (1)
	{
		i = 0;
		event_nb = epoll_wait(data->epoll_fd, surveil, 16, 10);
		if (!event_nb)
			send_ping(data);
		while (i < event_nb)
		{
			cursor = surveil[i];
			if (cursor.events & EPOLLERR)
				fetch_in_error_queue(data);
			if (cursor.events & EPOLLIN)
				receive_pong(data);
			++i;
		}
	}
}
