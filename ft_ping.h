#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <netinet/ip_icmp.h>

# define FLAG_VLOWER 1
# define FLAG_QM 2

typedef struct	s_data
{
	int		raw_sock;
	unsigned char	flags;
	char		*hostname;

}	t_data;

//init
void	create_socket(t_data *data);

//parsing
void	parse_retrieve_args(t_data *data, char **argv);

//end
void	ft_end(t_data *data, int exit_code);

//utils
void	error_end(t_data *data, char *msg, int exit_code);

#endif 
