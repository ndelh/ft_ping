#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/timerfd.h>
# include <linux/net_tstamp.h>
# include <sys/time.h>
# include <signal.h>
# include <netdb.h>
# include <resolv.h>
# include <netinet/ip_icmp.h>

# define FLAG_VLOWER 1
# define FLAG_QM 2
# define MIL 1000000

typedef union	u_force_align
{
	char	sequence_nb[128];
	struct cmsghdr align;
} t_force_align;

typedef struct	s_tab
{
	struct	timespec	send_time;
	int	already_used;
}	t_tab;

typedef struct	s_data
{
	int				raw_sock;
	int				epoll_fd;
	int				timerfd;
	int				nb_send;
	int				received;
	uint64_t			us_total_ms;
	uint64_t			square_total_ms;
	uint16_t			be_pid;
	char			*hostname;
	pid_t			pid;
	unsigned char	flags;
	struct s_tab	*sequence_tab;
	struct sockaddr	target_intel;
	struct icmphdr	*header;

}	t_data;

//init
void	init(t_data *data, char **s);
void	retrieve_target_intels(t_data *data);
void	sig_init(t_data *data);
void	epoll_init(t_data *data);
	//socket init
void	get_addr(t_data *data);
void	load_header(t_data *data);
void	open_raw_socket(t_data *data);

//parsing
void	parse_retrieve_args(t_data *data, char **argv);


//core
void	core_loop(t_data *data);
void	send_ping(t_data *data);
void	fetch_in_error_queue(t_data *data);
void	receive_pong(t_data *data);
//end
void	free_data(t_data *data);

//utils
	//string display
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	error_end(t_data *data, char *msg, int exit_code);
	//getter
t_data	*access_data(t_data *data);

#endif 
