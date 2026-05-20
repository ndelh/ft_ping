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
	struct timeval	tv;
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
				gettimeofday(&tv, 0);
				printf("timeval:%lu, %lu\n", tv.tv_sec, tv.tv_usec % 1000);
				printf("timestamp: %lu,%lu\n", all_ts[0].tv_sec, all_ts[0].tv_nsec % MIL);
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

void	dispatch_error(t_data *data, struct msghdr *msg)
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
				printf("seq: %u\n", seq_num);
				fetch_sent_time(data, msg, seq_num);
			}
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
	dispatch_error(data, &msg);
}