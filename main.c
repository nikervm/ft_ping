#include "ft_ping.h"

static void
init_ping(int arg_num, char **arguments)
{
    ping.fd = -1;
    ping.ttl = 64;
    ping.timeout = (struct timeval){1, 0};
    ping.user_address = get_address(arg_num, arguments);
    ping.destination = get_destination(ping.user_address);
}

int main(int ac, char **av)
{
    if (ac == 1)
        usage();
    init_ping(ac, av);
    create_socket();
    printf("fd: %d\n", ping.fd);
    return 0;
}
