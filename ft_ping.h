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
# define SOCKET_ERROR "Can't create socket\n"
# define SETSOCKOPT_ERROR "Can't set options to socket\n"
# define GETADDR_ERROR "Probably, the node/service is not known (nut there are plenty of errors in addrinfo)\n"

typedef struct s_pg {
    int                 fd;
    int                 flags;
    int                 ttl;
    struct timeval      time_out;
    char                *dist;
    struct sockaddr_in  sockaddr;
} t_pg;

t_pg ping;

void usage(void);
int parse_flags(char **arguments);
void get_address(int arg_num, char **arguments);
void get_ip(char *domain);
void error_exit(char *message);
void set_signals();
void header();

#endif
