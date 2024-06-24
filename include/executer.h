/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 08:46:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

# include <limits.h>
# include <sys/types.h>
# include <stdint.h>

// forward declarations
typedef struct s_data t_data;
typedef struct s_list t_list;
typedef struct s_ast_node t_ast_node;

int		execute_ast(t_ast_node *ast, t_data *data);
int 	execute_list(t_ast_node *ast, t_data *data);
int		execute_pipeline(t_ast_node *ast, t_data *data);
int		execute_command(t_list *tokenlist, t_data *data);
int     execute_redirection(t_ast_node *ast, t_data *data);
int     setup_redirect_stdout(t_list **tokenlist);
int		handle_first_child_process(t_data *data, t_ast_node *ast);
int		handle_second_child_process(t_data *data, t_ast_node *ast);
char	*create_path(char *base, char *path_env);
char	**get_argv_from_tokenlist(t_list *tokenlist);
int		resolve_command_path(char **argv, char *path_env);
int		count_tokens(t_list *tokenlist);
int		get_status_of_children(pid_t pid1, pid_t pid2);
int 	free_array(char **envpaths);
uint8_t	get_wait_status(int	status);

#  ifdef __cplusplus

}
#  endif

# endif
