/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:55:25 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:00:47 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

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

void	init_poll(t_data *data, struct pollfd *fds)
{
	fds->fd = data->timerfd;
	fds->events = POLLIN;
	fds->revents = 0;
	++fds;
	fds->fd = data->raw_sock;
	fds->events = POLLIN;
	fds->revents = 0;
}

void	core_loop(t_data *data)
{
	struct pollfd	fds[2];
	int				event_nb;

	set_timer_fd(data);
	init_poll(data, fds);
	print_begin(data);
	gettimeofday(&data->launch_time, 0);
	send_ping(data);
	while (data->end == 0)
	{
		event_nb = poll(fds, 2, -1);
		if (event_nb < 0 && errno != EINTR)
		{
			printf("problem detected\n");
			free_data(data);
			exit(1);
		}
		if (fds[0].revents & POLLIN && errno != EINTR)
			sec_routine(data);
		if (fds[1].revents & POLLIN)
			receive_pong(data);
	}
}
