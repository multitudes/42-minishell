/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 08:27:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

int		execute_ast(t_ast_node *ast, t_data *data);
int		execute_command(t_list *tokenlist, t_data *data);
char	*create_path(char *base, t_data *data);
int		resolve_command_path(char **argv, t_data *data);
int		count_tokens(t_list *tokenlist);
int 	free_array(char **envpaths);

#  ifdef __cplusplus

}
#  endif

# endif
