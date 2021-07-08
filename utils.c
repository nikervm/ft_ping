#include "ft_ping.h"

void
error_exit(char *message)
{
    printf("errno: %s\n", strerror(errno));
    fprintf(stderr, "%s", message);
    exit(0);
}

void
usage()
{
    printf(FT_PING_USAGE);
    exit(0);
}
