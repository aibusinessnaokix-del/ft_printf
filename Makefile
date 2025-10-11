CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libftprintf.a
SRCS = srcs/main.c
OBJS = $(SRCS:.c=.o)
INCS =  -I includes

all : $(NAME)

$(NAME) : $(OBJS)
	ar rc $(NAME) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS) $(NAME)

fclean:
	rm -f $(NAME) $(OBJS)

re:
	fclan all

bonus: 
	$(MAKE) all

.PHONEY: all clean fclean re