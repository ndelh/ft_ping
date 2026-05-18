#include "../../ft_ping.h"

void	initiate_data(t_data *data, struct msghdr *msg, struct iovec *iov, char *control)
{

	memset(msg, 0, sizeof(struct msghdr));
	memset(control, 0, 128);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = control;
	msg->msg_controllen = CMSG_SPACE(128);
}

void	fetch_sent_time(t_data *data, struct msghdr *msg, struct *cmsghdr cmsg, uint16_t seq)
{
	struct timespec	*timespec;

}

void	dispatch_error(t_data *data, struct msghdr *msg)
{
	struct cmsghr 			*cmsg;
	struct sock_extended_error	*err;
	uint16_t			seq_number;
	struct timespec			*timespec;

	cmsg = CMSG_FIRSTHDR(msg);
	if (!cmsg->cmsg_type == IP_RECVERR || !cmsg->cmsg_level == SOL_IP)
		return ;
	err = (sock_extend_error *)CMSG_DATA(cmsg);
	if (err->ee.erno == ENOMSG)
	{
		seq_number = (uint16_t)err->ee.info;
		fetch_sent_time(data, msg, cmsg, seq);
	}

}

void	fetch_in_error_queue(t_data *data)
{
	struct msghdr		msg;
	struct iovec		iov[1];
	union t_must_align	align;
	char			buffer[1024];

	memset(buffer, 0, 1024);
	iov[0].iov_base = buffer;
	iov[0].iov_len = sizeof(buffer);
	initiate_data(data, &msg, iov, align.control); 
	recvmsg(data->raw_sock, &msg, 0);
}

