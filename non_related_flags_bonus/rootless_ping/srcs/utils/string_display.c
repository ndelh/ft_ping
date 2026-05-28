/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:46:46 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 10:54:39 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

int	ft_strlen(char *s)
{
	char	*cursor;

	if (!s || !*s)
		return (0);
	cursor = s;
	while (*cursor)
		++cursor;
	return (cursor - s);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

void	print_end_intels(t_data *data)
{
	float		percent;
	int			lost;
	uint64_t	ms_duration;

	if (data->received == 0)
		data->min_time = 0;
	ms_duration = (data->last_act.tv_sec - data->launch_time.tv_sec) * 1000
		+ (data->last_act.tv_usec - data->launch_time.tv_usec) / 1000;
	printf("\n--- %s ping statistics ---\n", data->hostname);
	percent = 0;
	if (data->nb_send)
	{
		printf("%i packets transmitted, %i received, ",
			data->nb_send, data->received);
		lost = data->nb_send - data->received;
		percent = (float)lost * (float)100 / (float)data->nb_send;
	}
	if (data->error_nb)
		printf("+%i errors, ", data->error_nb);
	printf("%g%% packet loss, time %lums\n", percent, ms_duration);
	print_rtt(data);
}

void	print_begin(t_data *data)
{
	printf("PING %s (%s) %i(%i) bytes of data.\n",
		data->hostname, data->printable_ip, PAYLOAD_SIZE,
		PAYLOAD_SIZE + (int)(sizeof(struct icmphdr) + sizeof(struct iphdr)));
}
