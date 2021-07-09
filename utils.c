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
    printf("%s\n", FT_PING_USAGE);
    exit(0);
}
