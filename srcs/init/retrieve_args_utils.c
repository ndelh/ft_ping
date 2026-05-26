#include "../../ft_ping.h"

int	ft_atoi(char *s)
{
	int nb;

	nb = 0;
	while (*s && (*s <= '9' && *s >= '0'))
	{
		nb = nb * 10 + *s - '0';
		if (nb > 255)
		{
			nb = 0;
			break;
		}
		++s;
	}
	if (*s)
		nb = 0;
	return (nb);
}
