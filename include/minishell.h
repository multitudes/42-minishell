/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 12:56:48 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/22 15:52:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <stdbool.h>
# include <stdarg.h>
# include <libft.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include <limits.h>
# include "darray.h"
# include "environment.h"
# include "scanner.h"
# include "parser.h"
# include "history.h"
# include "analyser.h"
# include "executer.h"
# include "error.h"

/*
This allows us to use the debug macro to print debug messages but to 
compile them out when NDEBUG is defined.
If we define NDEBUG in the makefile or as a flag -DNDEBUG, 
the debug macro will be replaced with an empty macro.
during submission we will remove the debug macro from the code
including the macro below which is not allowed by norminette.
*/
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// for the history - can override with 
// -DMINIHISTFILE='"new relative path"' -DHISTSIZE=1000
// look at cat $HOME/.minishell_history
#define MINIHISTFILEPATH "/.minishell_history"
#define MINIHISTFILESIZE  1000

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

typedef struct s_darray 	t_darray;

/*
env_arr = enviroment variables array
ast = abstract syntax tree for parser as linked lists of nodes
DLESS_delimiter for DLESS
exit_status for the exit status of the last command
scanner_error when the mallocs in the scanner failed. 
keep track of how many tokens were created
pipe_open : am I in a piped process? the last process in a pipe will set it to zero
pipe_fd : file descriptors for the pipe
*/
struct s_data 
{
	const char	*input;
	t_list		*token_list;
	t_darray	*env_arr;
	t_ast_node	*ast;
	int			exit_status;
	bool		scanner_error;
	char		*scanner_err_str;
	int 		scanner_tokens_nr;
	bool		pipe_open;
	int 		pipe_fd[2];
};
typedef struct s_data t_data;

int		loop();
int		init_data(t_data **data);
void	free_data(t_data *data);

#  ifdef __cplusplus

}
#  endif

#endif