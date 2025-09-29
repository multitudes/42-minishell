# ============================================================================
# MINISHELL MAKEFILE
# ============================================================================

# Project name
NAME			:= minishell

# ============================================================================
# COMPILER & FLAGS
# ============================================================================
CC				:= cc
CFLAGS			:= -Wall -Werror -Wextra -g3
# CFLAGS		+= -DMINIHISTFILEPATH="\"/.minishell_test_history\""
# CFLAGS		+= -DNDEBUG  # Uncomment to disable debug prints

# ============================================================================
# DIRECTORIES
# ============================================================================
SRC_DIR			:= src
OBJ_DIR			:= obj
INC_DIR			:= include
LIBFT_DIR		:= lib/libft
TMP_DIR			:= /tmp/splash

# ============================================================================
# INCLUDES & LIBRARIES
# ============================================================================
INCLUDES		:= -I$(LIBFT_DIR) -I$(INC_DIR)
LIBFT			:= $(LIBFT_DIR)/libft.a
LDLIBS			:= -lm -lreadline -lcurses $(LIBFT)

# OS-specific configuration
UNAME			:= $(shell uname -s)
ifeq ($(UNAME), Linux)
	LDLIBS		+= -lbsd
else ifeq ($(UNAME), Darwin)
	BREW_PREFIX	:= $(shell brew --prefix readline)
	INCLUDES	+= -I$(BREW_PREFIX)/include
	LDLIBS		+= -L$(BREW_PREFIX)/lib
endif

# ============================================================================
# SOURCE FILES (Organized by module)
# ============================================================================

# Core files
CORE_SRCS		:= init.c fd.c main.c loop.c signals.c freedata.c

# History module
HISTORY_SRCS	:= history/history.c history/history2.c

# Scanner module
SCANNER_SRCS	:= scanner/scanner.c scanner/scanner_utils.c \
				   scanner/scanner_utils2.c scanner/scanner_utils3.c \
				   scanner/scanner_utils4.c scanner/scanner_error.c \
				   scanner/dollar_tokens.c scanner/token_functions.c \
				   scanner/token_functions2.c scanner/token_functions3.c \
				   scanner/reserved_builtins.c scanner/reserved_builtins2.c \
				   scanner/token_operators.c scanner/history_tokens.c \
				   scanner/token_blocks.c scanner/token_blocks2.c \
				   scanner/redirection_tokens.c scanner/redirection_tokens2.c

# Environment module
ENV_SRCS		:= environment/environment.c environment/environment2.c \
				   environment/environment3.c environment/environment4.c

# Parser module
PARSER_SRCS		:= parser/parser.c parser/parser2.c parser/parser3.c \
				   parser/parser4.c parser/parser_utils.c \
				   parser/parser_utils2.c parser/parser_utils3.c \
				   parser/parser_utils4.c

# Analyser module
ANALYSER_SRCS	:= analyser/analyser.c analyser/expansion_utils.c \
				   analyser/expansion_utils2.c analyser/expansion_quotes.c \
				   analyser/expansion_dollar.c analyser/expansion_tilde.c \
				   analyser/expansion_tilde2.c

# Utility modules
UTILS_SRCS		:= utils/utils.c utils/utils2.c
GLOBBING_SRCS	:= globbing/globbing.c globbing/globbing1.c
ERROR_SRCS		:= error/error_perror.c error/error_stderr.c
DARRAY_SRCS		:= darray/darray.c darray/darray2.c darray/darray3.c

# Builtins module
BUILTINS_SRCS	:= builtins/builtins.c builtins/builtin_cd.c \
				   builtins/builtin_echo.c builtins/builtin_env.c \
				   builtins/builtin_exit.c builtins/builtin_export.c \
				   builtins/builtin_pwd.c builtins/builtin_unset.c

# Executer module
EXECUTER_SRCS	:= executer/executer.c executer/executer2.c \
				   executer/executer3.c executer/executer4.c \
				   executer/executer5.c executer/executer6.c

# Heredoc module
HEREDOC_SRCS	:= heredoc/heredoc.c heredoc/heredoc2.c heredoc/heredoc3.c \
				   heredoc/heredoc4.c heredoc/heredoc5.c

# Combine all source files
SRCS			:= $(CORE_SRCS) $(HISTORY_SRCS) $(SCANNER_SRCS) $(ENV_SRCS) \
				   $(PARSER_SRCS) $(ANALYSER_SRCS) $(UTILS_SRCS) \
				   $(GLOBBING_SRCS) $(ERROR_SRCS) $(DARRAY_SRCS) \
				   $(BUILTINS_SRCS) $(EXECUTER_SRCS) $(HEREDOC_SRCS)

# Generate object files and add source directory prefix
SRCS			:= $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS			:= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Header files
HDRS			:= $(addprefix $(INC_DIR)/, init.h fd.h splash.h scanner.h \
				   environment.h parser.h analyser.h executer.h \
				   splash_error.h darray.h builtins.h globbing.h \
				   debug.h heredoc.h utils.h history.h)

# ============================================================================
# BUILD RULES
# ============================================================================

# Default target
.DEFAULT_GOAL	:= all

# Main targets
all: $(NAME) tests tests_integration bonus | setup_dirs
	@echo "✅ Build complete: $(NAME)"

# Setup directories
setup_dirs:
	@mkdir -p $(TMP_DIR)
	@chmod 700 $(TMP_DIR)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
	@mkdir -p $(@D)
	@echo "🔨 Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Build libft
$(LIBFT): | $(LIBFT_DIR)
	@echo "📚 Building libft..."
	@$(MAKE) -C $(LIBFT_DIR) all

# Ensure libft directory exists (handled by submodule now)
$(LIBFT_DIR):
	@echo "⚠️  Libft directory not found. Make sure submodules are initialized."
	@echo "    Run: git submodule update --init --recursive"
	@exit 1

# Link the main executable
$(NAME): $(LIBFT) $(OBJS)
	@echo "🔗 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDLIBS) -o $(NAME)

# ============================================================================
# UTILITY TARGETS
# ============================================================================

# Create bonus executable
bonus: $(NAME)
	@cp $(NAME) $(NAME)_bonus
	@echo "🎁 Bonus executable created: $(NAME)_bonus"

# Build and run tests
tests:
	@echo "🧪 Running unit tests..."
	@$(MAKE) -C tests

tests_integration:
	@echo "🧪 Running integration tests..."
	@$(MAKE) -C tests_integration

# Run monkey tests
monkey:
	@echo "🐒 Running monkey tests..."
	@sh monkey_tests/monkey.sh

# Check for dangerous functions
check:
	@echo "🔍 Checking for potentially dangerous functions..."
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SRCS) || echo "✅ No dangerous functions found"

# Show build information
info:
	@echo "📋 Build Information:"
	@echo "  Project: $(NAME)"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  OS: $(UNAME)"
	@echo "  Sources: $(words $(SRCS)) files"
	@echo "  Objects: $(words $(OBJS)) files"

# ============================================================================
# CLEAN TARGETS
# ============================================================================

clean:
	@echo "🧹 Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean 2>/dev/null || true
	@$(MAKE) -C tests clean 2>/dev/null || true

fclean: clean
	@echo "🧹 Full clean..."
	@rm -f $(NAME) $(NAME)_bonus
	@$(MAKE) -C $(LIBFT_DIR) fclean 2>/dev/null || true
	@$(MAKE) -C tests fclean 2>/dev/null || true
	@$(MAKE) -C tests_integration fclean 2>/dev/null || true

re: fclean all

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all clean fclean re tests tests_integration bonus monkey check info setup_dirs

# ============================================================================
# HELP TARGET
# ============================================================================

help:
	@echo "📖 Available targets:"
	@echo "  all              - Build everything (default)"
	@echo "  $(NAME)          - Build main executable"
	@echo "  bonus            - Create bonus executable"
	@echo "  tests            - Run unit tests"
	@echo "  tests_integration- Run integration tests"
	@echo "  monkey           - Run monkey tests"
	@echo "  clean            - Remove object files"
	@echo "  fclean           - Remove all generated files"
	@echo "  re               - Rebuild everything"
	@echo "  check            - Check for dangerous functions"
	@echo "  info             - Show build information"
	@echo "  help             - Show this help"