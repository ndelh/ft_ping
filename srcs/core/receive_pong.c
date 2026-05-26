#include "../../ft_ping.h"

uint64_t	treat_time(t_data *data, char *msg)
{
		uint64_t to_ret;
		struct timeval *ts;

		ts = (struct timeval *)msg;
		to_ret = 0;
		to_ret = (data->last_reply.tv_sec - ts->tv_sec) * 1000000 + (data->last_reply.tv_usec - ts->tv_usec);
		return (to_ret);
}

void	treat_msg(t_data *data, char *msg, ssize_t nb_read)
{
		uint64_t computed_time;

		nb_read -= data->current_ihl;
		if (nb_read < (ssize_t)(sizeof(struct icmphdr) + sizeof(struct timeval)))
		{
			printf("error with size of payload\n");
			return ;
		}
		computed_time = treat_time(data, msg + sizeof( struct icmphdr) + data->current_ihl);
		printf("%lu bytes from %s (%s): icmp_seq=%i ttl=%i time=%ld,%ldms\n", nb_read, data->true_name, data->printable_ip, data->current_seq, data->current_ttl, computed_time / 1000, computed_time % 1000);
		++data->received;
}

void	receive_pong(t_data *data)
{
	char	receive[1024];
	ssize_t			nb_read;
	socklen_t		size;
	struct sockaddr	from;


	memset(receive, 0, 1024);
	size = sizeof(struct sockaddr);
	nb_read = recvfrom(data->raw_sock, receive, sizeof(receive), 0, &from, &size);
	if (nb_read < 0)
	{
		perror("failed recvmsg");
		free_data(data);
		exit(1);
	}
	gettimeofday(&data->last_reply, 0);
	if (parse_received(data, receive, nb_read))
		return ;
	treat_msg(data, receive, nb_read);
}
