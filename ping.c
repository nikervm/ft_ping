#include "ft_ping.h"

void
display_header()
{
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		ping.user_address,
		ping.destination,
		DUMMY_DATA_BYTES,
		DUMMY_DATA_BYTES + ICMP_HEADER_BYTES);
}

void
display_result()
{
	printf("--- %s ping statistics ---",
		ping.user_address);
}

// https://i.stack.imgur.com/Yp1XM.png
static unsigned short
checksum(void *address, int len)
{
	unsigned short	*buff;
	unsigned long	sum;

	buff = (unsigned short *)address;
	sum = 0;
	while (len > 1)
	{
		sum += *buff;
		buff++;
		len -= sizeof(unsigned short);
	}
	if (len)
		sum += *(unsigned char *)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((unsigned short)~sum);
}

// https://titanwolf.org/Network/Articles/Article?AID=32323504-a2aa-4fff-a66b-bbed98e1e013#gsc.tab=0
static void
init_packet(s_packet *packet, struct timeval current_time)
{
	ft_bzero(packet, sizeof(s_packet));
	packet->header.icmp_type = ICMP_ECHO;
	packet->header.icmp_code = 0;
	packet->header.icmp_seq = ping.seq;
	packet->header.icmp_id = ping.id;
	ft_memcpy(&packet->header.icmp_dun, &(current_time.tv_sec), sizeof(current_time.tv_sec));
	packet->header.icmp_cksum = checksum(packet, sizeof(*packet));
}

static int
send_packet(s_packet *packet)
{
	if (sendto(ping.fd, packet, sizeof(*packet), 0, (struct sockaddr *)&ping.info, sizeof(ping.info)) <= 0) {
		// print to error dis (?)
		return 0;
	}
	return 1;
}

/*
 * http://ru.manpages.org/recvmsg/2
 *
 * struct iovec {				массив элементов приёма/передачи
 * 		void	*iov_base;		начальный адрес
 * 		size_t	iov_len;		количество передаваемых байт
 * };
 *
 * struct msghdr {
 * 		void			*msg_name;		необязательный адрес
 * 		socklen_t		msg_namelen;	размер адреса
 * 		struct iovec	*msg_iov;		массив приёма/передачи
 * 		size_t        	msg_iovlen;		количество элементов в msg_iov
 * 		void         	*msg_control;	вспомогательные данные
 * 		size_t        	msg_controllen; размер буфера вспомогательных данны[
 * 		int           	msg_flags;		флаги принятого сообщения
 */
static void
init_reply(s_reply *reply)
{
	ft_bzero(reply, sizeof(s_reply));
	reply->iov.iov_base = reply->receive_buffer;
	reply->iov.iov_len = sizeof(reply->receive_buffer);
	reply->header.msg_name = ping.destination;
	reply->header.msg_iov = &reply->iov;
	reply->header.msg_iovlen = 1;
	reply->header.msg_control = &reply->control;
	reply->header.msg_controllen = sizeof(reply->control);
}

static int
check_reply(s_reply *reply)
{
	return 1;
}

static int
receive_reply(s_reply *reply)
{
	if ((reply->received_bytes = recvmsg(ping.fd, &reply->header, 0)) > 0) {
		return check_reply(reply);
	} else {
		// print error std
		return 0;
	}
}

// https://github.com/amitsaha/ping/blob/master/ping.c
void
ping_loop()
{
	s_packet packet;
	s_reply reply;
	struct timeval s_timestamp;
	struct timeval e_timestamp;

	while (1)
	{
		if (gettimeofday(&s_timestamp, NULL) == -1) {
			error_exit(SETTIME_ERROR);
		}
		init_packet(&packet, s_timestamp);
		ping.transmitted++;
		if (send_packet(&packet) > 0) {
			init_reply(&reply);
			ping.received++;
		}
		ping.seq++;
		break;
	}
}
