#include "ft_ping.h"

void
handler_ctrl_c(int signal)
{
	print_result();
	exit(0);
}
