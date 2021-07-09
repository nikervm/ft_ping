#include "ft_ping.h"

static void
init_ping(int arg_num, char **arguments)
{
    ping.fd = -1;
    ping.user_address = get_address(arg_num, arguments);
    ping.destination = get_destination(ping.user_address);
}

int main(int ac, char **av)
{
    if (ac == 1)
        usage();
    init_ping(ac, av);
    return 0;
}
