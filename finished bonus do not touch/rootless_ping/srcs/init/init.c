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
	cap_t				caps;
	cap_flag_value_t	flags;
	
	caps = cap_get_proc();
	if (!caps)
	{
		perror("failed to retrieve capabilities");
		exit(1);
	}
	if (cap_get_flag(caps, CAP_NET_RAW, CAP_EFFECTIVE, &flags) == -1)
	{
		perror("failed cap_get_flags");
		cap_free(caps);
		exit(1);
	}
	cap_free(caps);
	if (flags != CAP_SET)
	{
			ft_putendl_fd("unsufficient capabilities to run ft_ping", 2);
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
