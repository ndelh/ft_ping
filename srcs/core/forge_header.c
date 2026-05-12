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
	gettimeofday((struct timeval *)payload, NULL);
	header->checksum = compute_checksum((unsigned short *)packet ,len);
}

void	send_ping(t_data *data)
{
	char	packet[sizeof(struct icmphdr) + 56];

	++data->nb_send;
	forge_content(data, packet, sizeof(packet));
	if (sendto(data->raw_sock, (void *)packet, sizeof(packet), 0, data->target_intel->ai_addr, sizeof(struct sockaddr)) == -1)
		printf("error");
	else
		printf("success");
}
