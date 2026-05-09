#include "../ft_ping.h"

void	treat_flags(t_data *data, char *opt_arg)
{
	if (!*opt_arg)
		error_end(data, "ping: -: Name or service not know", 0);
	while (*opt_arg)
	{
		if (*opt_arg == 'v')
			data->flags |= FLAG_VLOWER;
		if (*opt_arg == '?')
			data->flags |= FLAG_QM;
		else
			error_end(data, "ping: unsupported flag for ft_ping", 0);
	}
}

void	treat_no_option(t_data *data, char *arg)
{
	if (!data->hostname)
		data->hostname = strdup(arg);
	if (!data->hostname)
	{
		perror("failed malloc when trying to duplicate hostnameor hostadrr");
		ft_end(data, 1);
	}
}

void	parse_retrieve_args(t_data *data, char **argv)
{
	while (*argv)
	{
		if (**argv == '-')
			treat_flags(data, *argv + 1);
		else if (**argv)
			treat_no_option(data, *argv);
		else
			error_end(data, "ping: : Name or service not know", 0);
		++argv;
	}
}
