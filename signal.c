#include "ft_ping.h"
/*
** SIGABRT	                    Ошибка завершения
** SIGBREAK	                    Пользователь нажал клавиши CTRL-Break
** SIGFPE	                    Ошибка операции с плавающей запятой
** SIGILL	                    Неверная инструкция
** SIGINT	                    Пользователь нажал клавиши CTRL-C
** SIGSEGV	                    Некорректный доступ к памяти
** SIGTERM	                    Завершить программу
** SIGUSR1, SIGUSR2, SIGUSR3	Сигналы, определяемые пользователем
 */

static void
ctrl_c() {
    exit(0);
}

void
set_signals()
{
    signal(SIGINT, ctrl_c);
}
