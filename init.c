#include "ft_ping.h"

int
check_prev_arg(char *arg)
{
    // Проверить на - первый символ и
    // while по всем флагам и проверке
    // если флаг допускается
    return 1;
}

void
get_ip(char *domain)
{
    // https://masandilov.ru/network/guide_to_network_programming5
    // Также пример есть в man
    struct addrinfo hints;
    struct addrinfo *servinfo;
    char address[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;     // AF_UNSPEC для IPv4 или IPv6 одновременно
    hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
    if (getaddrinfo(domain, NULL, &hints, &servinfo) != 0)
        error_exit(GETADDR_ERROR);
    // преобразуем IP в строку
    memcpy(&ping.sockaddr, servinfo->ai_addr, sizeof(struct sockaddr_in));
    inet_ntop(AF_INET, &ping, address, INET_ADDRSTRLEN);
    // memcpy(&ping.ip, servinfo->ai_addr, sizeof(struct sockaddr))
}

void
get_address(int arg_num, char **arguments)
{
    int i = arg_num - 1;

    while (i > 0)
    {
        if (arguments[i][0] != '-' && check_prev_arg(arguments[i - 1]))
        {
            ping.dist = arguments[i];
            return;
        }
        i--;
    }
    error_exit(FT_PING_USAGE);
}

// idx нужен для флага c (как пример), если я его сделаю
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