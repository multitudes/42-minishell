/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:38:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 13:53:34 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// needed for the tests - leave it here
#ifdef __cplusplus
extern "C" {
#endif

# include "minishell.h"

/*
I am creating a ast tree for my context free grammar with these structs...
typedef enum	e_nodetype 
{
	NODE_EXPRESSION,
	NODE_TERMINAL,
	NODE_BUILTIN,
	NODE_COMMAND, 
	NODE_PIPE, 
	NODE_REDIRECT, 
	NODE_HEREDOC
} 				t_nodetype;

if it is a terminal node I will have the args
otherwise until it hasnt been expanded I will have the token list
after expansion finally the args will be filled
and it will be ready for the executer

typedef struct s_ast_node {
    t_nodetype type;
    struct s_ast_node* left;
    struct s_ast_node* right;
	// for now not used
    // char** args;
	t_list *token_list;
} t_ast_node;

my grammar will take a linked list of tokens of type 

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
*/

typedef enum	e_nodetype 
{
	NODE_EXPRESSION,
	NODE_TERMINAL,
	NODE_BUILTIN,
	NODE_COMMAND, 
	NODE_PIPE, 
	NODE_REDIRECT, 
	NODE_HEREDOC,
	NODE_NULL
} 				t_nodetype;

/*
if it is a terminal node I will have the args
otherwise until it hasnt been expanded I will have the token list
after expansion finally the args will be filled
and it will be ready for the executer
type is a ENUM of type t_nodetype
parent is a pointer to the parent node
left and right are pointers to the left and right nodes
token_list is a linked list of tokens
*/
typedef struct			s_ast_node {
    t_nodetype 			type;
	struct s_ast_node	*parent;
    struct s_ast_node	*left;
    struct s_ast_node	*right;
	t_list				*token_list;
}						t_ast_node;

t_ast_node	*create_ast(t_mini_data *data, t_list *token_list);
void		free_ast(t_ast_node *ast);
t_ast_node*	new_node(t_nodetype type, t_ast_node* left, t_ast_node* right, t_list *expr_token_list);
void 		print_ast(t_ast_node *ast);

#ifdef __cplusplus
}
#endif

#endif  // PARSER_H_
