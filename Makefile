CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g
CC = cc
INCLUDES = -I./lib/libft -I./include
SRCS = $(addprefix src/, main.c)

OBJS = $(SRCS:.c=.o)

NAME = minishell

LIBFTDIR = lib/libft
LIBFT = $(LIBFTDIR)/libft.a

LDLIBS = -lm -lreadline

UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
    LDLIBS += -lbsd
endif

# target
all: $(LIBFT) $(NAME) 

$(LIBFT):
	@if [ ! -d $(LIBFTDIR) ]; then \
	git clone https://github.com/multitudes/42-libft.git $(LIBFTDIR); \
	fi
	$(MAKE) -C $(LIBFTDIR) all

# Static pattern rule for compilation - with includes for the libft that will allow the <libft.h> notation 
$(OBJS) : %.o: %.c
	 $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

$(NAME): LDLIBS += $(LIBFT)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS) 

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean
	
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
