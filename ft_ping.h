#ifndef FT_PING
# define FT_PING

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <errno.h>
# include <math.h>

# define INVALID_FLAG   0b10000000
# define V_FLAG         0b00000001
# define H_FLAG		    0b00000010

# define FT_PING_USAGE "Usage: ft_ping [-v verbose] [-h help] destination\n"
# define INVALID_FLAG_ERROR		"ft_ping: invalid option -- '%c'\n"

typedef struct  s_pg {
    int         flags;
    char        *dist;
}               t_pg;

t_pg ping;

void usage(void);
int parse_flags(char **arguments);
void get_address(char **arguments);

#endif
