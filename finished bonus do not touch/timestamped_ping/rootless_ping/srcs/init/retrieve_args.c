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

void	treat_question(t_data *data, char **argv)
{
	if (!optopt)
		printf("ping helpers\n");
	else
		printf("%s : unrecognized argument :%c\n", *argv, optopt);
	free_data(data);
	exit(0);
}

void	retrieve_args(t_data *data, char **argv, int ac)
{
	int	opt;

	opterr = 0;
	while ((opt = getopt(ac, argv, "?vf")) != -1)
	{
		if (opt == 'v')
			data->flags |= FLAG_VLOWER;
		else if (opt == '?')
			treat_question(data, argv);
		else if (opt == 'f')
			data->flags |= FLAG_F;
	}
	retrieve_target(data, argv + 1);
}
