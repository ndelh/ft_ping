/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_display_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:46:53 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 10:53:22 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

uint64_t	compute_avg(t_data *data)
{
	if (data->received == 0)
		return (0);
	return (data->total_ms / data->received);
}

double	compute_mdev(t_data *data)
{
	double	avg_square;
	double	avg;
	double	varia;
	double	msec_mdev;

	if (data->received == 0)
		return (0);
	avg = (double)data->total_ms / data->received;
	avg_square = (double)data->square_total_ms / data->received;
	varia = avg_square - (avg * avg);
	if (varia < 0)
		varia = 0;
	msec_mdev = sqrt(varia);
	msec_mdev /= 1000.0;
	return (msec_mdev);
}

void	print_rtt(t_data *data)
{
	uint64_t	avg;
	double		mdev;

	avg = compute_avg(data);
	mdev = compute_mdev(data);
	printf("rtt min/avg/max/mdev = %lu.%lu/",
		data->min_time / 1000, data->min_time % 1000);
	printf("%lu.%lu/", avg / 1000, avg % 1000);
	printf("%lu.%lu/", data->max_time / 1000, data->max_time % 1000);
	printf("%.3f\n", mdev);
}
