/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:38:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 15:47:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANALYSER_H
# define ANALYSER_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif
# include "minishell.h"

/*
 * properties:
 * - equal_status is 0 default - 1 when the = is a var assignment and 
 * 2 when I have more than one equal sign and I dont need to expand tilde
 * - dollar_exp_front There is a dollar exp in the name of the var assignment 
 */
typedef struct  s_expansion_flags {
    int     equal_status;
    bool    dollar_exp_front;
}   t_exp_flags;

void	analyse_expand(t_ast_node *ast, t_data *data);
void	expand_tokenlist(t_data *data, t_list *tokenlist);
void	execute_expansion_by_type(t_data *data, t_list *tokenlist, t_exp_flags *flags);
void	expand_dollar(t_darray *env_arr, t_token *token);
char	*get_key(char *str);
char	*replace_dollar_vars(t_darray *env_arr, char *lexeme);
void	expand_path(t_darray *env_arr, t_list *tokenlist, t_exp_flags *flags);
void	expand_globbing(t_list *tokenlist);
void	expand_exit_status(t_data *data, t_token *token);
void	expand_single_quotes(t_token *token);
void	expand_double_quotes(t_data *data, t_token *token);
char	*get_home(t_darray *env_arr);
int     peek_is_valid_path(char c);
bool    valid_tilde_separator(char sep, int equal_status);
bool	valid_tilde_expansion(t_list *tokenlist, int index);
char	*replace_tilde_in_str(t_list *tokenlist, char *str, char *home, t_exp_flags *flags);
void	which_ast_node(t_ast_node *ast);
int	    count_chars_in_str(char *str, char c);
void	set_flags(t_list *tokenlist, t_exp_flags *flags);
void	reset_flags(t_exp_flags *flags);

#  ifdef __cplusplus

}
#  endif

# endif
