#include "ft_ping.h"

void
print_header()
{
	printf("PING %s (%s) %d(%d) bytes of data.\n",
		ping.user_address,
		ping.destination,
		DUMMY_DATA_BYTES,
		DUMMY_DATA_BYTES + ICMP_HEADER_BYTES);
}

static double
cal_mdev()
{
	double		average_time;
	double		average_squared_time;

	average_time = ping.sum_time / ping.transmitted;
	average_squared_time = ping.double_sum_time / ping.transmitted;
	return sqrt(fabs(average_squared_time - average_time * average_time));
}

/*
 * --- google.com ping statistics ---
 * 1 packets transmitted, 1 received, 0% packet loss, time 0ms
 * rtt min/avg/max/mdev = 14.777/14.777/14.777/0.000 ms
 */
void
print_result()
{
	if (ping.transmitted != 0) {
		if (gettimeofday(&ping.end_time, NULL) == -1) {
			error_exit(SETTIME_ERROR);
		}
		printf("\n--- %s ping statistics ---\n",
			   ping.user_address);
		if (!ping.invalid) {
			printf("%d packets transmitted, %d received, %d%% packet loss, time %.0lfms\n",
		  ping.transmitted,
		  ping.received,
		  100 - (ping.received * 100) / ping.transmitted,
		  time_diff(ping.start_time, ping.end_time));
		} else {
			printf("%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %.0lfms\n",
		  ping.transmitted,
		  ping.received,
		  ping.invalid,
		  100 - (ping.received * 100) / ping.transmitted,
		  time_diff(ping.start_time, ping.end_time));
		}
		if (ping.received != 0) {
			printf("rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n",
		  ping.min_time,
		  ping.sum_time / ping.transmitted,
		  ping.max_time,
		  // average diff between "ping"'s latency and average latency
		  cal_mdev());
		}
	}
}

static void
statistic(double diff)
{
	ping.sum_time += diff;
	ping.double_sum_time += diff * diff;
	if (ping.min_time > diff || ping.min_time == 0) {
		ping.min_time = diff;
	}
	if (ping.max_time < diff || ping.max_time == 0) {
		ping.max_time = diff;
	}
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
 * 		struct iovec	*msg_iov;		массив приёма/передачи / массив для scatter/gather
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
	struct ip	*packet_content;

	packet_content = (struct ip *)reply->receive_buffer;
	if (packet_content->ip_p != IPPROTO_ICMP) {
		if (ping.flags & V_FLAG)
			error_output(REPLY_ERROR);
		error_exit(REPLY_ERROR);
	}
	reply->icmp = (struct icmp *)(reply->receive_buffer + ((int)packet_content->ip_hl << 2));
	if (reply->icmp->icmp_type == 11 && reply->icmp->icmp_code == 0) {
		return -1;
	} else if (reply->icmp->icmp_id != ping.id || reply->icmp->icmp_seq != ping.seq) {
		init_reply(reply);
		return receive_reply(reply);
	}
	return 1;
}

int
receive_reply(s_reply *reply)
{
	if ((reply->received_bytes = recvmsg(ping.fd, &reply->header, 0)) > 0) {
		return check_reply(reply);
	} else {
		if ((errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		{
			if (ping.flags & V_FLAG)
				error_output(TIMEOUT_ERROR);
		}
		else
			error_output(RECV_ERROR);
		return 0;
	}
}

static void
print_seq(s_reply *reply, struct timeval start, struct timeval end)
{
	double diff = time_diff(start, end);

	if (ping.flags & D_FLAG) {
		printf("[%ld.%06d] ", end.tv_sec, end.tv_usec);
	}
	// 64 bytes from localhost (127.0.0.1): icmp_seq=3 ttl=64 time=0.045 ms
	printf("%d bytes from %s (%s) icmp_seq=%d ttl=%d time=%.*lf ms\n",
		reply->received_bytes,
		ping.user_address,
		ping.destination,
		ping.seq,
		ping.ttl,
		diff >= 1 ? 1 : 3,
		diff);
	statistic(diff);
}

static void
print_expired(s_reply *reply)
{
	struct ip *packet_content;
	char ip[INET_ADDRSTRLEN];
	char hostname[NI_MAXHOST];
	struct sockaddr_in tmp;

	// ttl = 1
	// From _gateway (192.168.101.1) icmp_seq=3 Time to live exceeded
	// ttl = 2
	// From lo247.core-01.lefort.countrycom.ru (213.251.247.129) icmp_seq=2 Time to live exceeded
	packet_content = (struct ip *)reply->receive_buffer;
	inet_ntop(AF_INET, &packet_content->ip_src, ip, INET_ADDRSTRLEN);
	tmp.sin_addr = packet_content->ip_src;
	tmp.sin_family = AF_INET;
	tmp.sin_port = 0;
	if (getnameinfo((struct sockaddr *)&tmp, sizeof(struct sockaddr_in),
			hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD) >= 0) {
		printf("From %s (%s): icmp_seq=%d Time to live exceeded\n",
		 hostname,
		 ip,
		 ping.seq);
	} else {
		printf("From %s: icmp_seq=%d Time to live exceeded\n",
		 ip,
		 ping.seq);
	}
}

static void
wait_interval(struct timeval start)
{
	struct timeval res;

	res.tv_sec = start.tv_sec + ping.interval;
	res.tv_usec = start.tv_usec;
	while (timercmp(&start, &res, <)) {
		if (gettimeofday(&start, NULL) == -1) {
			error_exit(SETTIME_ERROR);
		}
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

	if (gettimeofday(&ping.start_time, NULL) == -1) {
		error_exit(SETTIME_ERROR);
	}
	while (1)
	{
		if (gettimeofday(&s_timestamp, NULL) == -1) {
			error_exit(SETTIME_ERROR);
		}
		init_packet(&packet, s_timestamp);
		ping.transmitted++;
		if (send_packet(&packet) > 0) {
			init_reply(&reply);
			int res = receive_reply(&reply);
			if (res == 1) {
				ping.received++;
				if (gettimeofday(&e_timestamp, NULL) == -1) {
					error_exit(SETTIME_ERROR);
				}
				print_seq(&reply, s_timestamp, e_timestamp);
			} else if (res == -1) {
				ping.invalid++;
				print_expired(&reply);
			}
		}
		ping.seq++;
		if (ping.flags & C_FLAG) {
			ping.count--;
			if (ping.count <= 0) {
				break;
			}
		}
		wait_interval(s_timestamp);
	}
}
