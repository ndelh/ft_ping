#include "../ft_ping.h"

void	error_end(t_data *data, char *msg, int exit_code)
{
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
	ft_end(data, exit_code);
}
