/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_received.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:55:42 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:08:13 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	treat_error(t_data *data, uint8_t type,
		struct icmphdr *header, size_t nb_read)
{
	char			*s;
	struct iphdr	*ip;

	s = (char *)header;
	s += sizeof(struct icmphdr);
	ip = (struct iphdr *)s;
	if (ip->ihl * 4 >= nb_read)
		return ;
	s += ip->ihl * 4;
	nb_read -= (ip->ihl * 4 + sizeof(struct icmphdr));
	if (nb_read < sizeof(struct icmphdr))
		return ;
	header = (struct icmphdr *)s;
	if (header->un.echo.id != data->be_pid)
		return ;
	data->current_seq = ntohs(header->un.echo.sequence);
	if (type == ICMP_TIME_EXCEEDED)
		printf("From 'cannot dnqs in ret' icmp_seq=%i Time to live exceeded\n",
			data->current_seq);
	else if (type == ICMP_DEST_UNREACH)
		printf("Host: '%s' is unreachable\n", data->hostname);
	else if (type == ICMP_PARAMETERPROB)
		printf("corrupted request form\n");
	++data->error_nb;
}

int	parse_icmp_value(t_data *data, struct icmphdr *header, size_t nb_read)
{
	int	seq_num;

	if (header->type != ICMP_ECHOREPLY)
	{
		if (nb_read < sizeof(struct icmphdr) * 2 + sizeof(struct iphdr))
			return (1);
		treat_error(data, header->type, header, nb_read);
		return (1);
	}
	if (header->un.echo.id != data->be_pid)
		return (1);
	seq_num = ntohs(header->un.echo.sequence);
	data->current_seq = seq_num;
	return (0);
}

int	parse_received(t_data *data, char *s, ssize_t nb_read)
{
	struct icmphdr	*header_icmp;

	if (parse_size(data, s, nb_read))
		return (1);
	s += data->current_ihl;
	nb_read -= data->current_ihl;
	header_icmp = (struct icmphdr *)s;
	if (compute_checksum((unsigned short *)s, nb_read))
		return (1);
	if (parse_icmp_value(data, header_icmp, nb_read))
		return (1);
	return (0);
}
