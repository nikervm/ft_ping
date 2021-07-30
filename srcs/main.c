#include "ft_ping.h"

static void
init_ping(int arg_num, char **arguments)
{
	ping.count = 0;
	ping.interval = 1;
	ping.ttl = 64;
	ping.flags = parse_flags(arguments);
	if (ping.flags & H_FLAG || ping.flags & INVALID_FLAG) {
		usage();
	}
	ping.sum_time = 0;
	ping.min_time = 0;
	ping.max_time = 0;
	ping.double_sum_time = 0;
    ping.fd = -1;
    ping.seq = 1;
    ping.id = getpid();
    ping.transmitted = 0;
    ping.received = 0;
    ping.invalid = 0;
    ping.packet_size = -1;
    ping.timeout = (struct timeval){1, 0};
    ping.user_address = get_address(arg_num, arguments);
    ping.destination = get_destination(ping.user_address);
}

int main(int ac, char **av)
{
    if (ac == 1)
        usage();
    init_ping(ac, av);
    if (ping.flags & V_FLAG)
    	display_flags();
    create_socket();
    set_signals();
	print_header();
    ping_loop();
    print_result();
    free(ping.destination);
    exit(0);
}
