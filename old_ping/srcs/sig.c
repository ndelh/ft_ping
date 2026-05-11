#include "../ft_ping.h"

void	sigint_handler(int sig)
{
	(void)sig;
	printf("ctrl + c\n");
	ft_end(access_data(NULL), 0);
}

void	sigalarm_handler(int sig)
{
	(void)sig;
	printf("alarm\n");
	alarm(1);
}

void	set_sigint(t_data *data)
{
	struct sigaction	sa;

	sa.sa_handler = &sigint_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGALRM);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigint init failed");
		ft_end(data, 1);
	}
}

void	set_sigalarm(t_data *data)
{
	struct sigaction	sa;

	sa.sa_handler = &sigalarm_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
	{
		perror("sigalarm init failed");
		ft_end(data, 1);
	}
}

void	sig_init(t_data *data)
{
	set_sigint(data);
	set_sigalarm(data);
}
