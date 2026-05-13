#include "../../ft_ping.h"

void	receive_pong(t_data *data)
{
	char	receive[sizeof(struct iphdr) + sizeof(struct icmphdr) + 54];
	int	len;

	len = (sizeof(struct iphdr) + sizeof(struct icmphdr) + 54);
	recvfrom(data->raw_sock, receive, len, 0, NULL, 0);
	printf("success\n");
}
