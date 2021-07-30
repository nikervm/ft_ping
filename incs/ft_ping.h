#ifndef FT_PING
# define FT_PING

# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>
# include <sys/time.h>
//mac
# include <netinet/ip.h>
//
# include <netinet/ip_icmp.h>
# include <unistd.h>
# include <math.h>
# include <errno.h>

# define ADDR_ERROR			"ADDR_ERROR"
# define SOCKET_ERROR		"Can't create socket"
# define SETOPT_ERROR		"Error while setting socket"
# define SETTIME_ERROR		"Can't set the time"
# define REPLY_ERROR		"Invalid reply packet"
# define BAD_FLAG			"Invalid flag or parameter"
# define TIMEOUT_ERROR		"Request timed out"
# define RECV_ERROR			"Error while trying to access ICMP reply"

# define DUMMY_DATA_BYTES	56
# define ICMP_HEADER_BYTES	28
# define BUFF				64

# define C_FLAG 		0b00000001
# define I_FLAG 		0b00000010
# define T_FLAG 		0b00000100
# define D_FLAG 		0b00001000
# define F_FLAG 		0b00010000
# define H_FLAG 		0b00100000
# define V_FLAG 		0b01000000
# define INVALID_FLAG	0b10000000

typedef struct {
	struct icmp	header;
	char 		dummy_data[DUMMY_DATA_BYTES];
}	s_packet;

typedef struct {
	int 			received_bytes;
	struct msghdr	header;
	struct iovec	iov;
	struct icmp		*icmp;
	char 			receive_buffer[BUFF];
	char 			control[CMSG_SPACE(sizeof(int))];
}	s_reply;

typedef struct {
	struct timeval		start_time;
	struct timeval		end_time;
	double 				sum_time;
	double				double_sum_time;
	double 				min_time;
	double				max_time;
	int 				flags;
	int					fd;
	int 				ttl;
	int 				seq;
	pid_t				id;
	int 				count;
	int 				interval;
	int 				packet_size;
	int 				transmitted;
	int 				received;
	int 				invalid;
	struct timeval		timeout;
	char				*user_address;
	char				*destination;
	struct sockaddr_in	info;
}	s_ft_ping;

s_ft_ping ping;

/* init */
char *			get_address(int arg_num, char **arguments);
char *			get_destination(char *domain);
void			create_socket(void);
void 			set_signals(void);
int 			parse_flags(char **arguments);

/* utils */
void			usage(void);
void			error_exit(char *message);
void *			ft_memset(void *s, int c, size_t n);
void *			ft_memcpy(void *dest, const void *src, size_t len);
char *			ft_strdup(const char *string);
size_t			ft_strlen(const char *str);
void			ft_bzero(void *s, size_t len);
double			time_diff(struct timeval start, struct timeval end);
void			display_flags(void);
void			error_output(char *message);

/* signals */
void			handler_ctrl_c(int signal);

/* ping */
void			print_header(void);
void			print_result(void);
void			ping_loop(void);
int				receive_reply(s_reply *reply);

#endif
