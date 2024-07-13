/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/11 20:11:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include <limits.h>
# include <sys/types.h>
# include <stdint.h>
# include "scanner.h"
# include "darray.h"

// forward declarations
typedef struct s_data		t_data;
typedef struct s_list		t_list;
typedef struct s_ast_node	t_ast_node;

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

int execute_ast(t_ast_node * ast, t_data * data);
int		execute_list(t_ast_node *ast, t_data *data);
int		execute_pipeline(t_ast_node *ast, t_data *data);
int		execute_command(t_list *tokenlist, t_data *data);
uint8_t	execute_redirection(t_ast_node **ast);
uint8_t	setup_redirect(t_list **tokenlist, t_tokentype type);
int		handle_first_child_process(t_data *data, t_ast_node *ast);
int		handle_second_child_process(t_data *data, t_ast_node *ast);
char	*create_path(char *base, char *path_env);
char	**get_argv_from_tokenlist(t_list **tokenlist);
int		resolve_command_path(char **argv, char *path_env);
bool	find_path(char **argv, char *path_env); 
int		get_status_of_children(pid_t pid1, pid_t pid2);
int		free_array(char **envpaths);
uint8_t	get_wait_status(int status);
void	update_dollar_underscore(t_darray *env_arr, t_list *tokenlist);

#  ifdef __cplusplus

}
#  endif

# endif
