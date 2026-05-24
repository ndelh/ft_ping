#include "../../ft_ping.h"

void	initiate_struct(struct msghdr *msg, struct iovec *iov, char *control)
{

	memset(msg, 0, sizeof(struct msghdr));
	memset(control, 0, 128);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = control;
	msg->msg_controllen = 128;
}

//three timespec are used, software, hardware transformed(rarely used) and hardware. Last one require specific hardware

void	fetch_sent_time(t_data *data, struct msghdr *msg, uint16_t seq)
{
	struct timespec	all_ts[3];
	struct cmsghdr	*cmsg;
	void			*content;
	
	cmsg = CMSG_FIRSTHDR(msg);
	while (cmsg)
	{
		if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SO_TIMESTAMPING)
		{
			content = CMSG_DATA(cmsg);
			if (content)
			{
				memcpy(all_ts, content, sizeof(all_ts));
				data->sequence_tab[seq].in_use = 1;
				memcpy(&data->sequence_tab[seq].send_time, &all_ts[0], sizeof(struct timespec));
				return ;
			}
		}
		cmsg = CMSG_NXTHDR(msg, cmsg);
	}
}

uint16_t	fetch_seq_num(t_data *data)
{
		uint16_t	to_ret;

		if (data->queue_tail == data->queue_head)
		{
			ft_putendl_fd("major desync detected", 2);
			free_data(data);
			exit(1);
		}
		to_ret = data->my_queue[data->queue_head];
		data->queue_head = (data->queue_head + 1) % QUEUESIZE;
		--data->queue_size;
		return (to_ret);
}

int	parse_icmp(t_data *data, struct icmphdr *header)
{
	int	seq_num;
	
	if (header->un.echo.id != data->be_pid)
		return (1);
	seq_num = ntohs(header->un.echo.sequence);
	if (data->sequence_tab[seq_num].in_use)
		data->sequence_tab[seq_num].in_use = 0;
	else
	{
		printf("dup error\n");
		return (1);
	}
	data->current_seq = seq_num;
	return (0);
}

int		retrieve_seq_num(t_data *data, struct msghdr *msg, size_t msg_size)
{
		char	*s;
		struct icmphdr	*header;
		
		if (msg_size < sizeof(struct icmphdr))
			return (1);
		s = msg->msg_iov->iov_base;
		if (compute_checksum((unsigned short *)s, msg_size))
			return (1);
		header = (struct icmphdr *)s;
		if (parse_icmp(data, header))
			return (1);
		return (0);
}

void	treat_error(t_data *data, struct sock_extended_err *err, struct msghdr *msg, size_t nb_received)
{
	if (retrieve_seq_num(data, msg, nb_received))
		return ;
	++data->error_nb;
	switch(err->ee_type)
	{
		case(ICMP_TIME_EXCEEDED):
			printf("%s ttl_error icmp_seq:%i\n", data->true_name, data->current_seq);
			break;
		case(ICMP_DEST_UNREACH):
			printf("%s is unreachable icmp_seq:%i\n", data->true_name, data->current_seq);
			break ;
		default:
	}
	data->sequence_tab[data->current_seq].in_use = 0;

}

void	dispatch_error(t_data *data, struct msghdr *msg, size_t nb_received)
{
	struct cmsghdr 			*cmsg;
	struct sock_extended_err	*err;
	uint16_t					seq_num;

	cmsg = CMSG_FIRSTHDR(msg);
	while (cmsg)
	{
		if (cmsg->cmsg_type == IP_RECVERR && cmsg->cmsg_level == SOL_IP)
		{
			err = (struct sock_extended_err *)CMSG_DATA(cmsg);
			if (err->ee_origin == SO_EE_ORIGIN_TIMESTAMPING)
			{
				seq_num = fetch_seq_num(data);
				fetch_sent_time(data, msg, seq_num);
			}
			else if (err->ee_origin == SO_EE_ORIGIN_ICMP)
			 	treat_error(data, err, msg, nb_received);
		}
		cmsg = CMSG_NXTHDR(msg, cmsg);
	}
}

void	fetch_in_error_queue(t_data *data)
{
	struct msghdr		msg;
	struct iovec		iov[1];
	ssize_t				nb_received;
	t_force_align		align;
	char				buffer[1024];

	memset(buffer, 0, 1024);
	iov[0].iov_base = buffer;
	iov[0].iov_len = sizeof(buffer);
	initiate_struct(&msg, iov, align.content);
	nb_received = recvmsg(data->raw_sock, &msg, MSG_ERRQUEUE);
	if (nb_received < 0)
		return ;
	dispatch_error(data, &msg, nb_received);
}