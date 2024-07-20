# added -fPIE -fPIC because compiling on mac m1 and linux
# sometimes gave weird errors
CFLAGS=-fPIE -fPIC -Wall -Wextra -Werror
CC=cc

# part 1 - 23 functions
# part 2 - 11 functions
# bonus --  9 list functions
SOURCES= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
	ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
	ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_itoxp.c ft_itox.c ft_itoxx.c ft_atol.c

PRINTF_PATH = ft_printf

SOURCES_PRINTF = $(addprefix $(PRINTF_PATH)/, ft_printf.c pr_.c pr_c.c pr_s.c pr_p.c pr_x.c pr_xx.c \
base16_utils.c base16_utils2.c pr_i.c err.c pr_u.c utils.c utilsflags.c utilsflags2.c)
SOURCES += $(SOURCES_PRINTF)

GNL_PATH = get_next_line
SOURCES_GNL = $(addprefix $(GNL_PATH)/, get_next_line.c get_next_line_utils.c \
get_next_line_utils.c) 
SOURCES += $(SOURCES_GNL)

OBJECTS = $(SOURCES:.c=.o)

# this rule below is redundant but I leave it for clarity
# %.o: %.c 
# 	$(CC) $(CFLAGs) -c $< -o $@

NAME=libft.a
	
all: $(NAME)

# $@: Represents the target. In the context of my rule $(LIBFT): $(OBJECTS),
# it refers to $(LIBFT)
# $^: Represents all the prerequisites of the rule. 
# In the same context, $^ refers to $(OBJECTS)
# Then I have an ar command with the options 
# r (insert new files), c (create a new archive), 
# and s (write an object-file index into the archive).
$(NAME): $(OBJECTS)
	ar rcs $@ $^
	
clean:
	@echo ""
	@echo "==================\ncleaning all libft obj files and executables"
	rm -rf $(OBJECTS)
	@find . -name "*.dSYM" -delete -print
	@find . -name "*.c~" -delete -print
	@rm -f a.out

fclean: clean
	@echo
	@echo "==================\nRemoving ${NAME}"
	rm -f $(NAME)

re: fclean all

dev: CFLAGS=-g -Werror -Wall -Wextra
dev: all

