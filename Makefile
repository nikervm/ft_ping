NAME	=	ft_ping
SRCSDIR =	srcs
OBJSDIR =	objs
SRC	=	init.c	main.c	ping.c	signals.c	\
		utils.c
OBJS	=	$(SRCS:$(SRCSDIR)%.c=$(OBJSDIR)%.o)
SRCS	=	$(addprefix $(SRCSDIR)/, $(SRC))
INCSDIR	=	incs/

CC	=	gcc

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -lm -o $(NAME)

$(OBJSDIR)%.o: $(SRCSDIR)%.c
	@mkdir -p $(OBJSDIR)
	echo Compiling: $<
	$(CC) -MD -I$(INCSDIR) -lm -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
