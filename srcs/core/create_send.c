#include "../../ft_ping.h"

unsigned short compute_checksum(unsigned short *computing, int len)
{
	unsigned int	checksum;
	unsigned short	result;
	
	checksum = 0;
	while (len > 1)
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
	header->un.echo.sequence = htons((uint16_t)data->nb_send);
	data->my_queue[data->queue_tail] = (uint16_t)data->nb_send;
	data->queue_tail = (data->queue_tail + 1) % QUEUESIZE;
	++data->queue_size;
	payload = packet + sizeof(struct icmphdr);
	memcpy(payload, "ndelhota ping\n", 14);
	header->checksum = compute_checksum((unsigned short *)packet ,len);
}

void	setup_struct(t_data *data, struct msghdr *msg, struct iovec *iov)
{

	msg->msg_name = (struct sockaddr_in *)(&data->target_intel);
	msg->msg_namelen = sizeof(struct sockaddr_in);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = NULL;
	msg->msg_controllen = 0;
}

void	special_send(t_data *data, char *packet, int packet_size)
{
	struct	msghdr msg;
	struct iovec	iov[1];

	memset(&msg, 0, sizeof(struct msghdr));
	iov[0].iov_base = packet;
	iov[0].iov_len = packet_size;
	setup_struct(data, &msg, iov);
	if (sendmsg(data->raw_sock, &msg, 0) == -1)
	{
		perror("send_error");
		free_data(data);
		exit(1);
	}
}

void	send_ping(t_data *data)
{
	char	packet[sizeof(struct icmphdr) + PAYLOAD_SIZE];

	++data->nb_send;
	forge_content(data, packet, sizeof(packet));
	if (data->queue_size > QUEUESIZE)
	{
		ft_putendl_fd("systems block on send, queue size too big", 2);
		free_data(data);
		exit(1);
	}
	special_send(data, packet, sizeof(packet));
	if (data->flags & FLAG_F)
		write(1, ".", 1);
}
