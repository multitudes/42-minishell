/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:38:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/03 17:07:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANALYSER_H
# define ANALYSER_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif
# include "minishell.h"

void	analyse_expand(t_ast_node *ast, t_data *data);
void	expand_variable(t_darray *env_arr, t_token *token);
void	expand_globbing(t_list *tokenlist);
void	read_exit_status(t_data *data, t_token *token);
void	extract_string(t_token *token);
void	expand_string(t_data *data, t_token *token);
void	which_ast_node(t_ast_node *ast);

#  ifdef __cplusplus

}
#  endif

# endif