#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <netdb.h>
# include <resolv.h>
# include <netinet/ip_icmp.h>

# define FLAG_VLOWER 1
# define FLAG_QM 2

typedef struct	s_data
{
	int		raw_sock;
	unsigned char	flags;
	char		*hostname;
	struct addrinfo	*target_intel;

}	t_data;

//init
void	retrieve_target_intels(t_data *data);
void	sig_init(t_data *data);
void	create_socket(t_data *data);

//parsing
void	parse_retrieve_args(t_data *data, char **argv);

//end
void	ft_end(t_data *data, int exit_code);

//utils
void	error_end(t_data *data, char *msg, int exit_code);

#endif 
