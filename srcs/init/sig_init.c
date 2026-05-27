/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 11:20:00 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:20:16 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

void	sigint_handler(int sig)
{
	t_data	*data;

	data = access_data(NULL);
	data->end = sig;
}

void	sig_init(t_data *data)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &sigint_handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL))
	{
		perror("failed to initiate signal gestion");
		free_data(data);
		exit(0);
	}
}
