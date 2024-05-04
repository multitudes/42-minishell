/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 12:56:48 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/04 14:06:44 by lbrusa           ###   ########.fr       */
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
# include "environment.h"
# include "../lib/libft/libft.h"
# include "scanner.h"
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include <limits.h>
# include "handle_path.h"
# include "parser.h"
# include "history.h"
# include "analyser.h"
# include "executer.h"
# include "error.h"
# include "darray.h"

// This allows us to use the debug macro to print debug messages but to compile them out when NDEBUG is defined.
// If we define NDEBUG in the makefile or as a flag -DNDEBUG, the debug macro will be replaced with an empty macro.
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// for the history - can override with -DMINIHISTFILE='"new relative path"' -DHISTSIZE=1000
#define MINIHISTFILEPATH "/.minishell_history"
#define MINIHISTFILESIZE  1000

// needed for the tests - leave it here
#ifdef __cplusplus
extern "C" {
#endif

/*
enviroment list
path list
*/
struct s_mini_data 
{
	t_darray	*env_arr;
	t_ast_node	*ast;
	char		*heredoc_delimiter;
	int			exit_status;
};
typedef struct s_mini_data t_mini_data;

int		loop(int argc, char **argv);
int		init_data(t_mini_data **data);
void	free_data(t_mini_data *data);

#ifdef __cplusplus
}
#endif

# endif