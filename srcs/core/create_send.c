#include "../../ft_ping.h"

unsigned short compute_checksum(unsigned short *computing, int len)
{
	unsigned int	checksum;
	unsigned short	result;
	
	checksum = 0;
	while (len > 2)
	{
		checksum += *computing;
		++computing;
		len -= 2;
	}
	if (len)
		checksum += *(unsigned char *)computing;
	while (checksum >> 16)
		checksum = (checksum & 0xFFFF) + (checksum >> 16);
	result = (unsigned short)~checksum;
	return result;
}

void	forge_content(t_data *data, char *packet, int len)
{
	struct icmphdr	*header;
	char		*payload;	
	
	memset(packet, 0, len);
	header = (struct icmphdr *)packet;
	header->type = ICMP_ECHO;
	header->un.echo.id = data->be_pid;
	header->un.echo.sequence = htons(data->nb_send);
	payload = packet + sizeof(struct icmphdr);
	memcpy(payload, "ndelhota ping\n", 14);
	header->checksum = compute_checksum((unsigned short *)packet ,len);
}

void	setup_struct(t_data *data, struct msghdr *msg, struct iovec *iov, char *control)
{
	struct cmsghdr	*cmsg;

	msg->msg_name = (struct sockaddr_in *)(&data->target_intel);
	msg->msg_namelen = sizeof(struct sockaddr_in);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = control;
	msg->msg_controllen = CMSG_SPACE(sizeof(uint32_t));
	cmsg = CMSG_FIRSTHDR(msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_TIMESTAMPING;
	cmsg->cmsg_len = CMSG_LEN(sizeof(uint32_t));
	*(uint32_t *)CMSG_DATA(cmsg) = ((uint32_t)data->nb_send % UINT16_MAX) + 1; 
}

void	special_send(t_data *data, char *packet, int packet_size)
{
	struct	msghdr msg;
	struct iovec	iov[1];
	t_force_align	control;

	memset(&msg, 0, sizeof(struct msghdr));
	memset(&control, 0, sizeof(t_force_align));
	iov[0].iov_base = packet;
	iov[0].iov_len = packet_size;
	setup_struct(data, &msg, iov, control.sequence_nb);
	if (sendmsg(data->raw_sock, &msg, 0) == -1)
	{
		perror("send_error");
		free_data(data);
		exit(1);
	}
}

void	send_ping(t_data *data)
{
	char	packet[sizeof(struct icmphdr) + 56];

	++data->nb_send;
	forge_content(data, packet, sizeof(packet));
	special_send(data, packet, sizeof(packet));
}
