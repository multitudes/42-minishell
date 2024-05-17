/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 18:38:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/17 11:00:48 by lbrusa           ###   ########.fr       */
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
*/

typedef enum	e_nodetype 
{
	NODE_LIST,
	NODE_TRUE,
	NODE_FALSE,
	NODE_PIPELINE, 
	NODE_REDIRECT, 
	NODE_BUILTIN,
	NODE_COMMAND, 
	NODE_DLESS,
	NODE_TERMINAL,
	NODE_NULL
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
void		free_ast(t_ast_node *ast);
t_ast_node*	new_node(t_nodetype type, t_ast_node* left, t_ast_node* right, t_list *expr_token_list);
void 		print_ast(t_ast_node *ast);

#ifdef __cplusplus
}
#endif

#endif  // PARSER_H_
