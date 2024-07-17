/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:38:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/17 13:49:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include <stdbool.h>
# include "scanner.h"
# include "debug.h"
# include "parser.h"

// forwards declarations
typedef enum e_tokentype	t_tokentype;
/*
I am creating a ast tree for my context free grammar with these structs...
*/
typedef enum e_nodetype 
{
	NODE_NULL,
	NODE_LIST,
	NODE_PIPELINE, 
	NODE_COMMAND, 
	NODE_TERMINAL,
	NODE_BUILTIN,
}	t_nodetype;

/*
do I need the parent node? we will see when executing!
TODO - check later
*/
typedef struct s_ast_node {
	t_nodetype			type;
	struct s_ast_node	*parent;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_list				*tokenlist;
	int					original_stdout;
	int					original_stdin;
	int					original_stderr;
}						t_ast_node;

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

t_ast_node * create_ast(t_list * tokenlist);
t_ast_node	*new_node(t_nodetype type, t_ast_node *left, t_ast_node *right, \
			t_list *expr_tokenlist);
bool		movetonexttoken_andbreak(t_list **input_tokens);
bool		consume_token_and_connect(t_list **input_tokens);
t_ast_node	*parse_list(t_list **input_tokens);
t_ast_node	*parse_pipeline(t_list **input_tokens);
t_ast_node	*parse_terminal(t_list **input_tokens);
bool		extract_expression(t_list **head, t_list **input_tokens);
bool		replace_expression_tokens(t_list **head, t_list **input_tokens);
void		print_ast(t_ast_node *ast, int level);
void		print_token(t_list *input_token);
void		*free_ast(t_ast_node **ast);
void		print_token(t_list *input_token);
t_token		*get_curr_token(t_list *input_tokens);
char		*get_token_lexeme(t_list *input_tokens);
bool		token_followed_by_space(t_list *input_tokens);
t_tokentype	get_token_type(t_list *input_tokens);
bool		tokenlist_has_astnode(t_list *new_tokenlist);
bool		is_not_control_token(t_list *tokenlist);
int			count_list(t_list *input_tokens);
void		break_list(t_list **input_tokens);
bool		is_tree_control_token(t_list *tmp);
void		*get_b_node(t_ast_node **b, t_list **tokenlist, t_list *tmp);
bool		tokenize_expression_token(t_list **input_tokens, \
				t_list **expr_node, t_list **new_tokenlist, char **newlexeme);
void		*get_b_node_pipeline(t_ast_node **b, t_list **tokenlist, \
									t_list *tmp);
void		*parse_pipeline_innerloop(t_ast_node **a, t_list **tokenlist);
void		*parse_terminal_inner_loop(t_list **head, t_list **input_tokens, \
										bool *expr_has_node);
void		*parse_list_innerloop(t_ast_node **a, t_list **tokenlist);

#  ifdef __cplusplus

}
#  endif

# endif
