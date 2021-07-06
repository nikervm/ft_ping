#include "ft_ping.h"

void
get_address(char **arguments)
{
    int i = 1;

    while (arguments[i])
    {
        if (arguments[i][0] != '-')
        {
            ping.dist = arguments[i];
        }
    }
}

void
usage()
{
    printf(FT_PING_USAGE);
    exit(0);
}
