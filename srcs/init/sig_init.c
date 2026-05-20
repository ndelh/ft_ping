#include "../../ft_ping.h"

void	sigint_handler(int sig)
{
	(void)sig;
	free_data(access_data(NULL));
	printf("terminated\n");
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
