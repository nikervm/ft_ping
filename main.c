#include "ft_ping.h"

static void
init_ping(int arg_num, char **arguments)
{
    ping.fd = -1;
    ping.ttl = 64;
    ping.seq = 1;
    ping.id = getpid();
    ping.transmitted = 0;
    ping.received = 0;
    ping.invalid = 0;
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
    set_signals();
    display_header();
    ping_loop();
    return 0;
}
