#include "ft_ping.h"

static void
extract_flag(int *flags, char **arguments, const int idx)
{
    int i = 1;

    while (arguments[idx][i])
    {
        switch (arguments[idx][i])
        {
            case 'v':
                *flags |= V_FLAG;
                break;
            case 'h':
                *flags |= H_FLAG;
                break;
            default:
                *flags |= INVALID_FLAG;
                fprintf(stderr, INVALID_FLAG_ERROR, arguments[idx][i]);
                usage();
        }
        i++;
    }
}

int
parse_flags(char **arguments)
{
    int i = 1;
    int flags = 0;

    while (arguments[i])
    {
        if (arguments[i][0] == '-')
            extract_flag(&flags, arguments, i);
        i++;
    }
    return flags;
}