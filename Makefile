NAME			=	minishell

# flags
CFLAGS 			= 	-Wall -Werror -Wextra
CFLAGS		  	+=	-g3
# CFLAGS 	+= 	-DMINIHISTFILEPATH="\"/.minishell_test_history\""

# NDEBUG is a macro that disables the debug prints - uncomment to disable them
# comment out to enable them need to recompile with make re
# can use also like ./minishell 2>> error_log.txt to print stderror to a 
# file and have 
# all debug messages together
# CFLAGS += -DNDEBUG

CC 				=	cc

INCLUDES 		= 	-I./lib/libft -I./include

# directories
OBJ_DIR			= 	obj/
SRC_DIR			= 	src/
LIBFTDIR 		= 	lib/libft

# the files to compile
SRCS 			= 	$(addprefix $(SRC_DIR), main.c loop.c history/history.c history/history2.c \
scanner/scanner.c scanner/scanner_utils.c scanner/scanner_utils2.c scanner/scanner_utils3.c \
scanner/scanner_utils4.c scanner/scanner_utils5.c scanner/scanner_error.c \
scanner/token_functions.c scanner/token_functions2.c scanner/dollar_tokens.c scanner/reserved_builtins.c \
scanner/token_operators.c scanner/history_tokens.c scanner/token_blocks.c scanner/token_blocks2.c \
scanner/redirection_tokens.c environment/environment.c environment/environment2.c \
parser/parser.c parser/parser2.c parser/parser_utils.c parser/parser_utils2.c parser/parser_utils3.c \
analyser/analyser.c analyser/analyser2.c utils/utils.c utils/utils2.c globbing/globbing.c globbing/globbing1.c \
error.c error2.c darray/darray.c darray/darray2.c darray/darray3.c \
builtins/builtins.c builtins/builtins2.c executer/executer.c executer/executer2.c executer/executer3.c \
executer/executer4.c heredoc/heredoc.c heredoc/heredoc2.c heredoc/heredoc3.c)
OBJS 			=	$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))
HDRS 			=	$(addprefix include/, minishell.h scanner.h environment.h \
parser.h analyser.h executer.h splash_error.h darray.h builtins.h globbing.h debug.h heredoc.h) 

# linker flags and libraries
LIBFT 			=	$(LIBFTDIR)/libft.a
LDLIBS 			=	-lm -lreadline -lcurses

# OS specific flags
UNAME 			= 	$(shell uname -s)
ifeq ($(UNAME), Linux)
	LDLIBS 		+=  -lbsd
else ifeq ($(UNAME), Darwin)
	INCLUDES 	+= -I$(shell brew --prefix readline)/include
	LDLIBS 		+= -L$(shell brew --prefix readline)/lib
endif

# target
all: $(LIBFT) $(NAME) tests tests_integration copy_bonus

# Static pattern rule for compilation - adding the .o files in the obj folder 
# with includes for the libft that will allow the <libft.h> notation 
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Target $(LIBFT) depends on the libft library.
$(LIBFT):
	@if [ ! -d $(LIBFTDIR) ]; then \
	git clone https://github.com/multitudes/42-libft.git $(LIBFTDIR); \
	fi
	$(MAKE) -C $(LIBFTDIR) all

# Target $(NAME) depends on object files $(OBJS) and libft library.
$(NAME): LDLIBS += $(LIBFT)
$(NAME): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDLIBS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	#$(MAKE) -C $(LIBFTDIR) clean
	$(MAKE) -C tests clean
	
fclean: clean
	rm -f $(NAME)
	# $(MAKE) -C $(LIBFTDIR) fclean
	$(MAKE) -C tests fclean
	$(MAKE) -C tests_integration fclean

re: fclean all copy_bonus

tests:
	$(MAKE) -C tests

tests_integration:
	$(MAKE) -C tests_integration

monkey:
	sh monkey_tests/monkey.sh

copy_bonus: minishell
	cp minishell minishell_bonus

.PHONY: all clean fclean re tests tests_integration	copy_bonus

# This regex has been created by the maintainer of a http server 
# to avoid using c functions like strcpy() etc... 
# just for fun ill include it here 
# see more: http://www.and.org/and-httpd/
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SRCS) || true
