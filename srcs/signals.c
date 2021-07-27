#include "ft_ping.h"

void
handler_ctrl_c(int signal)
{
	print_result();
	free(ping.destination);
	exit(0);
}
