#include "ft_ping.h"

// http://www.faqs.org/rfcs/rfc1071.html
unsigned short
check_sum(void *addr, int size)
{
    /* Compute Internet Checksum for "size" bytes
    *         beginning at location "addr".
    */
    register long sum = 0;

    while (size > 1)  {
        /*  This is the inner loop */
        sum += *(unsigned short *)addr++;
        size -= sizeof(unsigned short);
    }

    /*  Add left-over byte, if any */
    if (size > 0)
        sum += * (unsigned char *)addr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return (unsigned short)~sum;
}

void
current_time(struct timeval *destination)
{
    if (gettimeofday(destination, NULL) == -1)
        error_exit(GET_TIME_ERROR);
}

void
error_exit(char *message)
{
    fprintf(stderr, "%s", message);
    exit(0);
}

void
usage()
{
    printf(FT_PING_USAGE);
    exit(0);
}
