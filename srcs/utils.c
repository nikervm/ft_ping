#include "ft_ping.h"

size_t
ft_strlen(const char *str)
{
	size_t len;
	unsigned int x;

	len = 0;
	while (1)
	{
		x = *(unsigned int *)str;
		if ((x & 0xff) == 0)
			return len;
		if ((x & 0xff00) == 0)
			return len + 1;
		if ((x & 0xff0000) == 0)
			return len + 2;
		if ((x & 0xff000000) == 0)
			return len + 3;
		str += 4;
		len += 4;
	}
}

char *
ft_strdup(const char *string)
{
	size_t len = ft_strlen(string) + 1;
	void *new = malloc(len);

	if (new == NULL)
		return NULL;
	return (char *)ft_memcpy(new, string, len);
}

void *
ft_memcpy(void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;

	for (int i = len; i > 0; i--) {
		*d++ = *s++;
	}
	return dest;
}

void *
ft_memset(void *addr, int sym_code, size_t len)
{
	unsigned char *ptr;

	ptr = (unsigned char *)addr;
	for (int i = len; i > 0; i--) {
		*(ptr++) = (unsigned char)sym_code;
	}
	return addr;
}

void
error_exit(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(0);
}

void
usage(void)
{
    printf("Usage\n"
				"\tft_ping [options] <destination>\n"
				"Options:\n"
				"\t-c <count>\t\t\tstop after <count> replies\n"
				"\t-i <interval>\t\tsecond between sending each packet\n"
				"\t-t <ttl>\t\t\tdefine time to live\n"
				"\t-s <size>\t\t\tuse <size> as number of data bytes to be sent\n"
				"\t-f\t\t\t\t\tflood ping\n");
    exit(0);
}

void
ft_bzero(void *s, size_t len)
{
	ft_memset(s, '\0', len);
}

double
time_diff(struct timeval start, struct timeval end)
{
	return ((double)end.tv_sec - (double)start.tv_sec) * 1000 + ((double)end.tv_usec - (double)start.tv_usec) / 1000;
}

void
error_output(char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

void
display_flags()
{
	printf("ft_ping: verbose mode activated");
	if (ping.flags & T_FLAG)
		printf(", IP Time to live set to %d", ping.ttl);
	if (ping.flags & C_FLAG)
		printf(", packet count set to %d", ping.count);
	if (ping.flags & I_FLAG)
		printf(", interval between packets set to %d", ping.interval);
	if (ping.flags & F_FLAG)
		printf(", flood mode activated");
	printf("\n");
}
