#include "../../ft_ping.h"

int	ft_strlen(char *s)
{
	char	*cursor;
	if (!s || !*s)
		return (0);
	cursor = s;
	while (*cursor)
		++cursor;
	return (cursor - s);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}
