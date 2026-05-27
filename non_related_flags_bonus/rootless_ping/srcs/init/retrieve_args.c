/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 11:17:05 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:18:47 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

void	retrieve_target(t_data *data, char **argv)
{
	while (*argv)
	{
		if (**argv && **argv != '-')
			data->hostname = *argv;
		++argv;
	}
}

void	print_ping_opt(t_data *data)
{
	write(1, "\n", 1);
	ft_putendl_fd("Usage", 1);
	ft_putendl_fd("  ping [options] <destination>", 1);
	write(1, "\n", 1);
	ft_putendl_fd("Options:", 1);
	printf("  %-20s%s\n", "<destination>", "DNS name or IP adress");
	printf("  %-20s%s\n", "-q", "quiet output");
	printf("  %-20s%s\n", "-t <ttl>", "define time to live");
	printf("  %-20s%s\n", "-v", "verbose output");
	free_data(data);
	exit(0);
}

void	treat_question(t_data *data, char **argv)
{
	if (!optopt)
		print_ping_opt(data);
	else
		printf("%s : unrecognized argument: '%c'\n", *argv, optopt);
	free_data(data);
	exit(0);
}

void	treat_t(t_data *data, char *opt, char **argv)
{
	data->current_ttl = ft_atoi(opt);
	if (data->current_ttl == 0)
	{
		printf("%s : invalid argument: '%s': out of range: 0 "
			"<= value <= 255\n", *argv, opt);
		free_data(data);
		exit(0);
	}
}

void	retrieve_args(t_data *data, char **argv, int ac)
{
	int	opt;

	opterr = 0;
	opt = getopt(ac, argv, "q?vft:");
	while (opt != -1)
	{
		if (opt == 'v')
			data->flags |= FLAG_VLOWER;
		else if (opt == '?')
			treat_question(data, argv);
		else if (opt == 'q')
			data->flags |= FLAG_Q;
		else if (opt == 't')
			treat_t(data, optarg, argv);
		opt = getopt(ac, argv, "q?vft:");
	}
	retrieve_target(data, argv + 1);
}
