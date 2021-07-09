#ifndef FT_PING
# define FT_PING

# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <arpa/inet.h>

# define FT_PING_USAGE		"Usage: ft_ping [-v verbose] [-h help] destination"
# define ADDR_ERROR			"ADDR_ERROR"

typedef struct {
	int					fd;
	char				*user_address;
	char				*destination;
	struct sockaddr_in	info;
}	s_ft_ping;

s_ft_ping ping;

/* init */
char *			get_address(int arg_num, char **arguments);
char *			get_destination(char *domain);

/* utils */
void			usage(void);
void			error_exit(char *message);
void *			ft_memset(void *s, int c, size_t n);
void *			ft_memcpy(void *dest, const void *src, size_t len);
char *			ft_strdup(const char *string);
size_t			ft_strlen(const char *str);

#endif
