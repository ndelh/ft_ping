#ifndef FT_PING_H
# define FT_PING_H

//include order count must be posix to net to linux specifics
# include <sys/types.h>
# include <sys/socket.h> 
# include <sys/epoll.h>
# include <sys/timerfd.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <resolv.h>
# include <arpa/inet.h>
# include <linux/net_tstamp.h>
# include <linux/errqueue.h>

# define FLAG_VLOWER 1
# define FLAG_F 2
# define BIL 1000000000
# define QUEUESIZE 10
# define PAYLOAD_SIZE 56

typedef union	u_force_align
{
	char	content[128];
	struct cmsghdr align;
} t_force_align;

typedef struct	s_tab
{
	struct	timespec	send_time;
	int					in_use;
}	t_tab;

typedef struct	s_data
{
	int				raw_sock;
	int				epoll_fd;
	int				timerfd;
	int				nb_send;
	int				received;
	int				error_nb;
	uint64_t			total_ms;
	uint64_t			square_total_ms;
	uint64_t			max_time;
	uint64_t			min_time;
	uint16_t			be_pid;
	char			*hostname;
	pid_t			pid;
	unsigned char	flags;
	struct s_tab	*sequence_tab;
	struct sockaddr	target_intel;
	struct icmphdr	*header;
	struct timeval	launch_time;
	struct timeval	last_reply;
	char					true_name[NI_MAXHOST];
	char					printable_ip[INET_ADDRSTRLEN];
	uint16_t				my_queue[QUEUESIZE];
	uint16_t				queue_head;
	uint16_t				queue_tail;
	uint16_t				queue_size;
	uint8_t					current_ihl;
	uint8_t					current_ttl;
	uint16_t				current_seq;
}	t_data;

//init
void	init(t_data *data, char **s, int ac);
void	retrieve_target_intels(t_data *data);
void	sig_init(t_data *data);
void	epoll_init(t_data *data);
void	retrieve_args(t_data *data, char **argv, int ac);
	//socket init
void	get_addr(t_data *data);
void	open_raw_socket(t_data *data);

//parsing
void	parse_retrieve_args(t_data *data, char **argv);


//core
void	core_loop(t_data *data);
void	send_ping(t_data *data);
void	fetch_in_error_queue(t_data *data);
void	receive_pong(t_data *data);
int		parse_received(t_data *data, char *s, ssize_t nb_read);
unsigned short compute_checksum(unsigned short *computing, int len);
//special loop
void	core_loop_f(t_data *data);
//end
void	free_data(t_data *data);

//utils
	//string display
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	error_end(t_data *data, char *msg, int exit_code);
void	print_end_intels(t_data *data);
void	print_begin(t_data *data);
	//getter
t_data	*access_data(t_data *data);

#endif 
