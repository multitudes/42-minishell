# NDEBUG is a macro that disables the debug prints - uncomment to disable them
# comment out to enable them need to recompile with make re
# can use also like ./minishell 2>> error_log.txt to print stderror to a file and have 
# all debug messages together
CFLAGS += -DNDEBUG

CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CXX = c++

# the file to tests (SUT = System Under Test)
SUT_SRCS = $(addprefix ../src/, loop.c history.c scanner/scanner.c scanner/scanner_utils.c scanner/scanner_utils2.c scanner/scanner_utils3.c scanner/scanner_utils4.c environment.c \
handle_path.c parser.c analyser.c executer.c error.c darray.c)
SUT_HDRS = $(addprefix ../include/, minishell.h scanner.h environment.h handle_path.h parser.h analyser.h executer.h error.h darray.h) 
# will be compiled into object files
SUT_OBJS = $(SUT_SRCS:.c=.o)

LDLIBS =	-lm -lreadline -lcurses
LDLIBS += -L../lib/libft -lft
INCLUDES = -I../lib/libft -I../include

UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDLIBS += -lbsd
else ifeq ($(UNAME), Darwin)
	INCLUDES += -I$(shell brew --prefix readline)/include
	LDLIBS += -L$(shell brew --prefix readline)/lib
endif

# my test files - each one has a main and will 
# be compiled into an executable
TEST_SRCS= minishell_basic1_tests.cpp minishell_scanner_tests.cpp builtin_scanner_tests.cpp \
ident_scanner_delimiter_tests.cpp subst_scanner_tests.cpp operator_scanner_tests.cpp \
parser_tree_tests.cpp darray_env_tests.cpp lists_tests.cpp

# this substitution create files without the extension
# they will become the executables
TESTS=$(patsubst %.cpp,%,$(TEST_SRCS))


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

fclean: clean
	rm -rf $(TESTS)

re: fclean tests

.PHONY: tests 

#will not work on mac
valgrind:
	@echo
	@echo "==================\nLaunching valgrind..."
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)
