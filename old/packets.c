#include "ft_ping.h"

// https://a-khakimov.github.io/posts/ping-in-c/
// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol#header_code
// https://datatracker.ietf.org/doc/html/rfc1071
// https://datatracker.ietf.org/doc/html/rfc792
// http://www.faqs.org/rfcs/rfc1071.html
static ping_packet
create_packet(struct timeval cur_time)
{
    ping_packet packet;

    bzero(&packet, sizeof(packet));
    packet.header.icmp_type = ICMP_ECHO;
    packet.header.icmp_code = 0;
    packet.header.icmp_hun.ih_idseq.icd_seq = ping.seq;
    packet.header.icmp_hun.ih_idseq.icd_id = ping.own_id;;
    memcpy(&packet.header.icmp_dun.id_ts, &(cur_time), sizeof(cur_time));
    packet.header.icmp_cksum = check_sum(&packet, sizeof(ping_packet));

    return packet;
}

static ping_answer
init_answer()
{
    ping_answer answer;

    bzero(&answer, sizeof(ping_answer));
    answer.iov.iov_base = answer.bytes_buff;
    answer.iov.iov_len = sizeof(answer.bytes_buff);
    answer.header.msg_name = ping.address;
    answer.header.msg_iov = &answer.iov;
    answer.header.msg_iovlen = 1;
    answer.header.msg_control = &answer.control;
    answer.header.msg_controllen = sizeof(answer.control);

    return answer;
}

static void
get_answer(ping_answer answer)
{
    answer.bytes = recvmsg(ping.fd, &answer.header, 0);
    return;
}

static int
send_packet(ping_packet packet)
{
    ssize_t bytes;

    bytes = sendto(ping.fd, &packet, sizeof(packet), 0, (struct sockaddr *)&ping.info, sizeof(struct sockaddr));
    if (bytes <= 0)
    {
        error_exit("IDK WHAT TO WRITE\n");
    }
    return 1;
}

// ping использует тип 8 и 0 (ICMP_ECHO и ICMP_ECHOREPLY, эхо-запрос и эхо-ответ)
void
send_packets()
{
    ping_packet packet;
    ping_answer answer;
    struct timeval curr_start_time;
    struct timeval curr_end_time;

    current_time(&ping.start_time);
    while (1)
    {
        current_time(&curr_start_time);
        packet = create_packet(curr_start_time);
        ping.send_num++;
        send_packet(packet);
        answer = init_answer();
        get_answer(answer);
        return;
    }
}
