#include "ft_ping.h"

static void
get_value(char **argv, int i, char flag, int *value)
{
	if (argv[i][1] == flag && argv[i][2] == '\0' && argv[i + 1] != NULL) {
		*value = atoi(argv[i + 1]);
		return;
	}
	error_exit(BAD_FLAG);
}

static void
get_flag(int *flags, char **argv, int i)
{
	for (int j = 1; argv[i][j]; j++) {
		switch (argv[i][j]) {
			case 'c':
				*flags |= C_FLAG;
				get_value(argv, i, 'c', &ping.count);
				break;
			case 'i':
				*flags |= I_FLAG;
				get_value(argv, i, 'i', &ping.interval);
				break;
			case 't':
				*flags |= T_FLAG;
				get_value(argv, i, 't', &ping.ttl);
				break;
			case 'D':
				*flags |= D_FLAG;
				break;
			case 'f':
				*flags |= F_FLAG;
				break;
		}
	}
}

int
parse_flags(char **argv)
{
	int flags = 0;

	for (int i = 1; argv[i]; i++) {
		if (argv[i][0] == '-') {
			get_flag(&flags, argv, i);
		}
	}
	return flags;
}

static int
check_prev_arg(const char *argument, int pos)
{
	// -c 1 localhost
	// проверка для других флагов
	if (pos == 1) {
		return 1;
	}
	if (argument[0] != '-') {
		return 1;
	}
	if (argument[1] != 'c' && argument[1] != 'i' && argument[1] != 't') {
		return 1;
	}
	return 0;
}

char *
get_address(int arg_num, char **arguments)
{
    for (int i = arg_num - 1; i > 0; i--) {
        if (arguments[i][0] != '-' && check_prev_arg(arguments[i - 1], i)) {
            return arguments[i];
        }
    }
    usage();
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

void
set_signals()
{
	signal(SIGINT, &handler_ctrl_c);
}
