/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:38:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 11:28:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

/*
I am creating a ast tree for my context free grammar with these structs...
*/

typedef enum	e_nodetype 
{
	NODE_NULL,
	NODE_LIST,
	NODE_PIPELINE, 
	NODE_COMMAND, 
	NODE_TERMINAL,
	NODE_BUILTIN,
} 				t_nodetype;

/*
do I need the parent node? we will see when executing!
TODO - check later
*/
typedef struct			s_ast_node {
    t_nodetype 			type;
	struct s_ast_node	*parent;
    struct s_ast_node	*left;
    struct s_ast_node	*right;
	t_list				*token_list;
}						t_ast_node;

t_ast_node	*create_ast(t_list *token_list);
t_ast_node	*new_node(t_nodetype type, t_ast_node *left, t_ast_node *right, \
			t_list *expr_token_list);
t_ast_node	*parse_list(t_list **input_tokens);
t_ast_node	*parse_pipeline(t_list **input_tokens);
t_ast_node	*parse_terminal(t_list **input_tokens);
bool		extract_expression(t_list **head, t_list **input_tokens);
void		print_ast(t_ast_node *ast, int level);
void		*free_ast(t_ast_node **ast);
void		print_token(t_list *input_token);
t_token		*get_curr_token(t_list *input_tokens);
char		*get_token_lexeme(t_list *input_tokens);
t_tokentype	get_token_type(t_list *input_tokens);
bool		token_list_has_astnode(t_list *new_token_list);
bool		is_not_control_token(t_token *token);


#  ifdef __cplusplus

}
#  endif

# endif
