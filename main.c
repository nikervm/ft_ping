#include "ft_ping.h"

//static void
//ctrl_c() {
//    exit(0);
//}

static void
init_ping(char **arguments)
{
    ping.flags = parse_flags(arguments);
    get_address(arguments);
}

int
main(int argc, char **argv)
{
    if (argc == 1)
        usage();
    init_ping(argv);

    return 0;
}
