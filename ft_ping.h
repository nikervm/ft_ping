#ifndef FT_PING
# define FT_PING

# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <signal.h>
# include <sys/time.h>
# include <netinet/ip_icmp.h>
# include <unistd.h>

# define FT_PING_USAGE		"Usage: ft_ping [-v verbose] [-h help] destination"
# define ADDR_ERROR			"ADDR_ERROR"
# define SOCKET_ERROR		"Can't create socket"
# define SETOPT_ERROR		"Error while setting socket"
# define SETTIME_ERROR		"Can't set the time"
# define REPLY_ERROR		"Invalid reply packet"

# define DUMMY_DATA_BYTES	56
# define ICMP_HEADER_BYTES	28
# define BUFF				64

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
	int					fd;
	int 				ttl;
	int 				seq;
	pid_t				id;
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

/* utils */
void			usage(void);
void			error_exit(char *message);
void *			ft_memset(void *s, int c, size_t n);
void *			ft_memcpy(void *dest, const void *src, size_t len);
char *			ft_strdup(const char *string);
size_t			ft_strlen(const char *str);
void			ft_bzero(void *s, size_t len);

/* signals */
void			handler_ctrl_c(int signal);

/* ping */
void			display_header(void);
void			ping_loop(void);

#endif
