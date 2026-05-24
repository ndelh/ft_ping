#include "../../ft_ping.h"

int	parse_size(t_data *data, char *s, ssize_t nb_read)
{
	struct iphdr	*ip_header;

	if (nb_read < (ssize_t)(sizeof(struct iphdr)))
		return (1);
	ip_header = (struct iphdr *)s;
	data->current_ihl = ip_header->ihl * 4;
	data->current_ttl = ip_header->ttl;
	nb_read -= data->current_ihl;
	if (nb_read < (ssize_t)(sizeof(struct icmphdr)))
		return (1);
	return (0);
}

int	parse_icmp_value(t_data *data, struct icmphdr *header)
{
	int	seq_num;
	
	if (header->un.echo.id != data->be_pid)
		return (1);
	if (header->type != ICMP_ECHOREPLY)
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

int	parse_received(t_data *data, char *s, ssize_t nb_read)
{
	struct icmphdr *header_icmp;

	if (parse_size(data, s, nb_read))
		return (1);
	s += data->current_ihl;
	header_icmp = (struct icmphdr *)s;
	if (compute_checksum((unsigned short *)s ,nb_read))
		return (1);
	if (parse_icmp_value(data, header_icmp))
		return (1);
	return (0);
}
