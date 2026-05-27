/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 11:13:29 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:15:58 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

void	pre_check(void)
{
	if (getuid() != 0)
	{
		ft_putendl_fd("ft_ping: too few right,"
			"this program must be run as root", 2);
		exit(1);
	}
}

void	init(t_data *data, char **argv, int ac)
{
	memset(data, 0, sizeof(t_data));
	data->raw_sock = -1;
	data->epoll_fd = -1;
	data->be_pid = htons(getpid());
	pre_check();
	retrieve_args(data, argv, ac);
	open_raw_socket(data);
	get_addr(data);
	sig_init(data);
	data->min_time = UINT64_MAX;
}
