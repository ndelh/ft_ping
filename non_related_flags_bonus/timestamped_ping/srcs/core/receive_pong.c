#include "../../ft_ping.h"

void	init_msg_struct(struct msghdr *msg, struct iovec *iov, char *control)
{
	memset(msg, 0, sizeof(struct msghdr));
	memset(control, 0, 128);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = control;
	msg->msg_controllen = 128;
}

uint64_t	compute_time(t_data *data, struct timespec *rec_time)
{
		uint64_t s_time;
		uint64_t	r_time;
		uint64_t	to_ret;
		struct timespec	*send_time;

		send_time = &data->sequence_tab[data->current_seq].send_time;
		to_ret = 0;
		s_time = send_time->tv_sec * BIL + send_time->tv_nsec; 
		r_time = rec_time->tv_sec * BIL + rec_time->tv_nsec;
		to_ret = r_time - s_time;
		to_ret /= 1000;
		++data->received;
		data->total_ms += to_ret;
		if (to_ret < data->min_time)
			data->min_time = to_ret;
		if (to_ret > data->max_time)
			data->max_time = to_ret;
		data->square_total_ms += (to_ret * to_ret);
		return (to_ret);
}

uint64_t	retrieve_time(t_data *data, struct msghdr *msg)
{
		struct cmsghdr *cmsg;
		struct timespec ts[3];
		void			*content;

		cmsg = CMSG_FIRSTHDR(msg);
		while (cmsg)
		{
			if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SO_TIMESTAMPING)
			{
				content = CMSG_DATA(cmsg);
				if (content)
				{
					memcpy(ts, content, sizeof(struct timespec) * 3);
					return (compute_time(data, &ts[0]));
				}
			}
			CMSG_NXTHDR(msg, cmsg);
		}
		return (0);
}

void	treat_msg(t_data *data, struct msghdr *msg, ssize_t nb_read)
{
		uint64_t computed_time;

		if (parse_received(data, msg->msg_iov->iov_base, nb_read))
			return;
		computed_time = retrieve_time(data, msg);
		nb_read -= (data->current_ihl);
		printf("%lu bytes from %s (%s): icmp_seq=%i ttl=%i time=%ld,%ldms\n", nb_read, data->true_name, data->printable_ip, data->current_seq, data->current_ttl, computed_time / 1000, computed_time % 1000);
}

void	receive_pong(t_data *data)
{
	char	receive[1024];
	struct iovec	iov[1];
	struct msghdr	msg;
	t_force_align	align;
	ssize_t			nb_read;

	memset(receive, 0, 1024);
	iov[0].iov_base = receive;
	iov[0].iov_len = 1024;
	init_msg_struct(&msg, iov, align.content);
	nb_read = recvmsg(data->raw_sock, &msg, 0);
	if (nb_read < 0)
	{
		perror("failed recvmsg");
		free_data(data);
		exit(1);
	}
	gettimeofday(&data->last_act, 0);
	treat_msg(data, &msg, nb_read);
}
