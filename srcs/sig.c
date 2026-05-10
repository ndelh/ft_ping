#include "../ft_ping.h"

void	sigint_handler(int sig)
{
	(void)sig;
	printf("ctrl + c\n");
}

void	sigalarm_handler(int sig)
{
	(void)sig;
	printf("alarm\n");
	alarm(1);
}

void	sig_init(t_data *data)
{
	struct sigaction	sa;

	sa.sa_handler = &sigalarm_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &sa, NULL);
	sa.sa_handler = &sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaddset(&sa.sa_mask, SIGALRM);
	sigaction(SIGINT, &sa, NULL);
	(void)data;
}
