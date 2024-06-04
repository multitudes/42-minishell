# NDEBUG is a macro that disables the debug prints - uncomment to disable them
# comment out to enable them need to recompile with make re
# can use also like ./minishell 2>> error_log.txt to print stderror to a file and have 
# all debug messages together
# CFLAGS 		+= 	-DNDEBUG

CXXFLAGS 	= 	-Wall -Wextra -Werror -std=c++98
CXXFLAGS 	+= 	-g
CXX = c++

# directories
OBJ_DIR		= 	../obj/
SRC_DIR		= 	../src/

# the file to tests (SUT = System Under Test)
SUT_SRCS 	= 	$(addprefix $(SRC_DIR), loop.c history/history.c history/history2.c \
scanner/scanner.c scanner/scanner_utils.c scanner/scanner_utils2.c scanner/scanner_utils3.c \
scanner/scanner_utils4.c scanner/scanner_utils5.c scanner/scanner_error.c \
scanner/token_functions.c scanner/dollar_tokens.c scanner/reserved_builtins.c \
scanner/token_operators.c scanner/token_blocks.c scanner/history_tokens.c \
scanner/redirection_tokens.c scanner/token_blocks2.c environment/environment.c environment/environment2.c \
parser/parser.c parser/parser2.c parser/parser_utils.c parser/parser_utils2.c parser/parser_utils3.c analyser.c utils.c	\
error.c darray/darray.c darray/darray2.c darray/darray3.c globbing.c \
builtins/builtins.c builtins/builtins2.c executer/executer.c executer/executer1.c executer/executer2.c)
SUT_HDRS 	= 	$(addprefix ../include/, minishell.h scanner.h environment.h \
parser.h analyser.h executer.h error.h darray.h builtins.h globbing.h) 

# will be compiled into object files
SUT_OBJS 	= 	$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SUT_SRCS))

# linker flags and libraries	
LDLIBS 		=	-lm -lreadline -lcurses
LDLIBS 		+=	-L../lib/libft -lft
INCLUDES 	=	-I../lib/libft -I../include

# OS specific flags
UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDLIBS 		+= -lbsd
else ifeq ($(UNAME), Darwin)
	INCLUDES 	+= -I$(shell brew --prefix readline)/include
	LDLIBS 		+= -L$(shell brew --prefix readline)/lib
endif

# my test files - each one has a main and will 
# be compiled into an executable
TEST_SRCS	=	minishell_basic1_tests.cpp scanner_tests.cpp \
builtin_scanner_tests.cpp ident_scanner_delimiter_tests.cpp \
subst_scanner_tests.cpp operator_scanner_tests.cpp parser_tree_tests.cpp \
darray_env_tests.cpp lists_tests.cpp globbing_tests.cpp unset_builtin_tests.cpp \

# this substitution create files without the extension
# they will become the executables
TESTS		=	$(patsubst %.cpp,%,$(TEST_SRCS))

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# the target build
all: tests
	
tests: $(TESTS) runtests

$(TESTS): $(SUT_OBJS) $(TEST_SRCS) $(SUT_HDRS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(SUT_OBJS) $@.cpp $(LDLIBS) -o $@

runtests:
	sh ./runtests.sh

clean:
	rm -f *.o
	rm -f tests.log
	find . -name "*.dSYM" -type d -exec rm -rf {} +

fclean: clean
	rm -rf $(TESTS)

re: fclean tests

.PHONY: tests 

#will not work on mac
valgrind:
	@echo
	@echo "==================\nLaunching valgrind..."
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)
