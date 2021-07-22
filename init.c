#include "ft_ping.h"

static int
check_prev_arg(char *argument)
{
	// -c 1 localhost
	// проверка для других флагов
	return 1;
}

char *
get_address(int arg_num, char **arguments)
{
    for (int i = arg_num - 1; i > 0; i--) {
        if (arguments[i][0] != '-' && check_prev_arg(arguments[i - 1])) {
            return arguments[i];
        }
    }
    error_exit(FT_PING_USAGE);
}

char *
get_destination(char *domain)
{
	struct addrinfo hints;
	struct addrinfo *infos;
	char ip_string[INET_ADDRSTRLEN];

	ft_memset(&hints, 0, sizeof(hints));
	// IPv4
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(domain, NULL, &hints, &infos) != 0) {
		error_exit(ADDR_ERROR);
	}
	ft_memcpy(&ping.info, infos->ai_addr, sizeof(struct sockaddr_in));
	inet_ntop(AF_INET, (void *)&ping.info.sin_addr, ip_string, sizeof(ip_string));
	freeaddrinfo(infos);
	return ft_strdup(ip_string);
}

void
create_socket()
{
	int fd;

	if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
		error_exit(SOCKET_ERROR);
	}
	if ((setsockopt(fd, IPPROTO_IP, IP_TTL, &ping.ttl, sizeof(ping.ttl))) == -1) {
		error_exit(SETOPT_ERROR);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&ping.timeout, sizeof(ping.timeout))) {
		error_exit(SETOPT_ERROR);
	}
	ping.fd = fd;
}
