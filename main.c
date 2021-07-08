#include "ft_ping.h"

static void
init_ping(int arg_num, char **arguments)
{
    ping.fd = -1;
    ping.ttl = 64;
    ping.time_out = (struct timeval) {10, 0};
    ping.flags = parse_flags(arguments);
    get_address(arg_num, arguments);
    get_ip(ping.dist);
}

static void
create_socket()
{
    int fd;

    // Посмотрел в другом гите
    // Создаёт "сырой" сокет и кидаются на него настройки
    // В частности время жизни пакета и тайм аут
    // Про это прочитать можно в 7 главе Стивенса
    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
        error_exit(SOCKET_ERROR);
    if (setsockopt(fd, IPPROTO_IP, IP_TTL, &(ping.ttl), sizeof(ping.ttl)) == -1)
        error_exit(SETSOCKOPT_ERROR);
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &(ping.time_out), sizeof(ping.time_out)) == -1)
        error_exit(SETSOCKOPT_ERROR);
    ping.fd = fd;
}

int
main(int argc, char **argv)
{
    if (argc == 1)
        usage();
    init_ping(argc, argv);
    if (ping.dist)
        create_socket();
    if (ping.fd >= 0)
    {
        set_signals();
        header();
    }
    return 0;
}
