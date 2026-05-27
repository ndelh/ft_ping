#include "../../ft_ping.h"

void	sigint_handler(int sig)
{
	struct timeval	end_time;
	
	(void)sig;
	gettimeofday(&end_time, 0);
	free_data(access_data(NULL));
	print_end_intels(access_data(NULL));
	exit(0);
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
