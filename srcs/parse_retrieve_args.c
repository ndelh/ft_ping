#include "../ft_ping.h"

void	treat_flags(t_data *data, char *opt_arg)
{
	if (!*opt_arg)
		error_end(data, "ping: -: Name or service not know", 0);
	while (*opt_arg)
	{
		if (*opt_arg == 'v')
			data->flags |= FLAG_VLOWER;
		else if (*opt_arg == '?')
			data->flags |= FLAG_QM;
		else
			error_end(data, "ping: unsupported flag for ft_ping", 0);
		++opt_arg;
	}
}

void	parse_retrieve_args(t_data *data, char **argv)
{
	char	*target;
	while (*argv)
	{
		if (**argv == '-')
			treat_flags(data, *argv + 1);
		else if (**argv)
			target = *argv;
		else
			error_end(data, "ping: : Name or service not know", 0);
		++argv;
	}
	data->hostname = strdup(target);
	if (!data->hostname)
		error_end(data, "failed malloc when retrieving hostname", 0);	
}
