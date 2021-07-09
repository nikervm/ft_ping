#include "ft_ping.h"

void
header()
{
    printf("PING %s (%s) %d(%d) bytes of data.\n",
           ping.dist,
           ping.address,
           (int)PING_PACKET_SIZE,
           (int)(PING_PACKET_SIZE + (int)sizeof(struct icmp)));
}
