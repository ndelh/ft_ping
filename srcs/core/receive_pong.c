/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receive_pong.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:56:14 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:12:20 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

uint64_t	treat_time(t_data *data, char *msg)
{
	uint64_t		to_ret;
	struct timeval	*ts;

	ts = (struct timeval *)msg;
	to_ret = 0;
	to_ret = (data->last_act.tv_sec - ts->tv_sec) * 1000000
		+ (data->last_act.tv_usec - ts->tv_usec);
	if (data->min_time > to_ret)
		data->min_time = to_ret;
	if (data->max_time < to_ret)
		data->max_time = to_ret;
	data->total_ms += to_ret;
	data->square_total_ms += to_ret * to_ret;
	return (to_ret);
}

void	treat_msg(t_data *data, char *msg, ssize_t nb_read)
{
	uint64_t	computed_time;

	nb_read -= data->current_ihl;
	if (nb_read < (ssize_t)(sizeof(struct icmphdr) + sizeof(struct timeval)))
	{
		printf("error with size of payload\n");
		return ;
	}
	computed_time = treat_time(data,
			msg + sizeof(struct icmphdr) + data->current_ihl);
	++data->received;
	if (data->flags & FLAG_Q)
		return ;
	printf("%lu bytes from %s (%s): icmp_seq=%i ttl=%i time=%.3fms\n",
		nb_read, data->true_name, data->printable_ip,
		data->current_seq, data->current_ttl, (double)computed_time / 1000.0);
}

void	receive_pong(t_data *data)
{
	char			receive[1024];
	ssize_t			nb_read;
	socklen_t		size;
	struct sockaddr	from;

	memset(receive, 0, 1024);
	size = sizeof(struct sockaddr);
	nb_read = recvfrom(data->raw_sock, receive,
			sizeof(receive), 0, &from, &size);
	if (nb_read < 0)
	{
		perror("failed recvmsg");
		free_data(data);
		exit(1);
	}
	gettimeofday(&data->last_act, 0);
	if (parse_received(data, receive, nb_read))
		return ;
	treat_msg(data, receive, nb_read);
}
