/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_send.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:55:35 by ndelhota          #+#    #+#             */
/*   Updated: 2026/05/27 11:09:42 by ndelhota         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_ping.h"

unsigned short	compute_checksum(unsigned short *computing, int len)
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
	result = (unsigned short) ~ checksum;
	return (result);
}

void	forge_content(t_data *data, char *packet, int len)
{
	struct icmphdr	*header;
	struct timeval	ts;
	char			*payload;	

	memset(packet, 0, len);
	header = (struct icmphdr *)packet;
	header->type = ICMP_ECHO;
	header->un.echo.id = data->be_pid;
	header->un.echo.sequence = htons((uint16_t)data->nb_send);
	payload = packet + sizeof(struct icmphdr);
	gettimeofday(&ts, 0);
	memcpy(payload, &ts, sizeof(struct timeval));
	header->checksum = compute_checksum((unsigned short *)packet, len);
}

void	send_ping(t_data *data)
{
	char	packet[sizeof(struct icmphdr) + PAYLOAD_SIZE];

	++data->nb_send;
	forge_content(data, packet, sizeof(packet));
	sendto(data->raw_sock, packet, sizeof(packet), 0,
		&data->target_intel, sizeof(struct sockaddr));
	gettimeofday(&data->last_act, 0);
}
