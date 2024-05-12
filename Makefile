CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g

# NDEBUG is a macro that disables the debug prints - uncomment to disable them
# comment out to enable them need to recompile with make re
# can use also like ./minishell 2>> error_log.txt to print stderror to a file and have 
# all debug messages together
# CFLAGS += -DNDEBUG

CC = cc
INCLUDES = -I./lib/libft -I./include

SRCS = $(addprefix src/, main.c loop.c history.c scanner/scanner.c scanner/scanner_utils.c scanner/scanner_utils2.c scanner/scanner_utils3.c scanner/scanner_utils4.c scanner/scanner_utils5.c scanner/scanner_error.c scanner/token_functions.c scanner/dollar_tokens.c scanner/reserved_builtins.c scanner/token_operators.c environment.c handle_path.c parser.c analyser.c executer.c error.c darray.c)
OBJS = $(SRCS:.c=.o)
HDRS = $(addprefix include/, minishell.h scanner.h environment.h handle_path.h parser.h analyser.h executer.h error.h darray.h) 
NAME = minishell

LIBFTDIR = lib/libft
LIBFT = $(LIBFTDIR)/libft.a

LDLIBS = -lm -lreadline -lcurses

UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDLIBS += -lbsd
else ifeq ($(UNAME), Darwin)
	INCLUDES += -I$(shell brew --prefix readline)/include
	LDLIBS += -L$(shell brew --prefix readline)/lib
endif

# target
all: $(LIBFT) $(NAME) tests

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@if [ ! -d $(LIBFTDIR) ]; then \
	git clone https://github.com/multitudes/42-libft.git $(LIBFTDIR); \
	fi
	$(MAKE) -C $(LIBFTDIR) all

# Static pattern rule for compilation - with includes for the libft that will allow the <libft.h> notation 
$(OBJS) : %.o: %.c
	 $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

# Target $(NAME) depends on object files $(OBJS) and libft library.
$(NAME): LDLIBS += $(LIBFT)
$(NAME): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDLIBS) -o $(NAME)

clean:
	rm -f $(OBJS)
# $(MAKE) -C $(LIBFTDIR) clean
	
fclean: clean
	rm -f $(NAME)
#$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

tests:
	$(MAKE) -C tests

.PHONY: all clean fclean re tests

# This regex has been created by the maintainer of a http server 
# to avoid using c functions like strcpy() etc... 
# just for fun ill include it here 
# see more: http://www.and.org/and-httpd/
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SRCS) || true
