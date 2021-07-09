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


# define PING_PACKET_SIZE   56
# define BUFFER_PING        64
# define INVALID_FLAG       0b10000000
# define V_FLAG             0b00000001
# define H_FLAG		        0b00000010

# define FT_PING_USAGE "Usage: ft_ping [-v verbose] [-h help] destination\n"
# define INVALID_FLAG_ERROR		"ft_ping: invalid option -- '%c'\n"
# define SOCKET_ERROR "Can't create socket\n"
# define SETSOCKOPT_ERROR "Can't set options to socket\n"
# define GETADDR_ERROR "Probably, the node/service is not known (but there are plenty of errors in addrinfo)\n"
# define GET_TIME_ERROR "Get time error\n"

typedef struct {
    int             bytes;
    struct msghdr   header;
    struct iovec    iov;
    struct icmp     *icmp;
    char            bytes_buff[BUFFER_PING];
    char            control[CMSG_SPACE(sizeof(int))];
}       ping_answer;

typedef struct {
    struct icmp header;
    char        data[PING_PACKET_SIZE];
}       ping_packet;

typedef struct {
    int                 fd;
    int                 flags;
    int                 ttl;
    struct timeval      time_out;
    char                *dist;
    char                *address;
    int                 seq;
    pid_t               own_id;
    struct sockaddr_in  info;
    struct timeval      start_time;
    struct timeval      end_time;
    int                 send_num;
    int                 receive_num;
} t_pg;

t_pg ping;

void usage(void);
int parse_flags(char **arguments);
void get_address(int arg_num, char **arguments);
void get_ip(char *domain);
void error_exit(char *message);
void set_signals();
void header();
void send_packets();
void current_time(struct timeval *t);
unsigned short check_sum(void *address, int size);

#endif
