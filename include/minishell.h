/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 12:56:48 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 16:14:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// stdio.h below is definitely needed by macos because of the 
// alternative brew readline library used in the project
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/errno.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <stdbool.h>
# include <stdarg.h>
# include <assert.h>
# include <limits.h>
# include <libft.h>
# include "darray.h"
# include "environment.h"
# include "parser.h"
# include "history.h"
# include "executer.h"

// global.h
extern int g_signal;

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

// for the history - can override with 
// -DMINIHISTFILE='"new relative path"' -DHISTSIZE=1000
// look at cat $HOME/.minishell_history
// if not defined, the default is:
#ifndef MINIHISTFILEPATH
# define MINIHISTFILEPATH "/.minishell_history"
#endif

#ifndef MINIHISTFILESIZE
# define MINIHISTFILESIZE  1000
#endif

/*
env_arr = enviroment variables array
ast = abstract syntax tree for parser as linked lists of nodes
exit_status for the exit status of the last command
scanner_error when the mallocs in the scanner failed. 
keep track of how many tokens were created
pipe_open : am I in a piped process? the last process in a pipe will set it to zero
pipe_fd : file descriptors for the pipe
*/
struct s_data 
{
	// char 		*home;	
	const char	*input;
	t_list		*token_list;
	t_darray	*env_arr;
	t_ast_node	*ast;
	uint8_t		exit_status;
	bool		scanner_error;
	char		*scanner_err_str;
	int 		scanner_tokens_nr;
	bool		pipe_open;
	int 		pipe_fd[2];
	int			original_stdout;
	int			original_stdin;
	int			original_stderr;
};
typedef struct s_data t_data;

typedef struct s_darray 	t_darray;

int			loop();
bool		init_data(t_data **data);
bool    	init_data2(t_data **data);
bool		init_env_darray(t_darray **env_array);
void		free_data(t_data **data);
void		update_env_exit_status_with(uint8_t exit_status, t_data *data);
int			single_command(const char *input);
int			set_up_signals(void);
void		save_fds(t_data *data);
void		restore_fds(t_data *data);

#  ifdef __cplusplus

}
#  endif

#endif
