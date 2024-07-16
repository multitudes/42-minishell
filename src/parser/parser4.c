/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:17:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 14:55:44 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

/*
 * checks if the token is a pipe or pipe_and token
 */
bool	is_pipe_token(t_list *input_tokens)
{
	t_token	*token;

	if (input_tokens == NULL)
		return (false);
	token = (t_token *)input_tokens->content;
	if (token->type == PIPE || token->type == PIPE_AND)
		return (true);
	return (false);
}

/*
 * function called in parse list inner loop
 * to save lines of code- returns the b node or NULL
 * I dont need to return a node but I need to return a pointer
 * to catch the NULL return 
 */
void	*get_b_node_pipeline(t_ast_node **b, t_list **tokenlist, t_list *tmp)
{
	if (!movetonexttoken_andbreak(tokenlist))
	{
		ft_lstdelone(tmp, free_tokennode);
		return (NULL);
	}
	*b = parse_terminal(tokenlist);
	if (*b == NULL)
	{
		ft_lstdelone(tmp, free_tokennode);
		return (NULL);
	}
	return (*b);
}

/*
 * function called in parse pipeline 
 * to save lines of code- returns the new node or NULL
 * I dont need to return a node but I need to return a pointer
 * to catch the NULL return 
 */
void	*parse_pipeline_innerloop(t_ast_node **a, t_list **tokenlist)
{
	t_ast_node	*b;
	t_list		*tmp;
	t_ast_node	*tmpnode;

	while (is_pipe_token(*tokenlist))
	{
		tmp = *tokenlist;
		if (!get_b_node_pipeline(&b, tokenlist, tmp))
			return (free_ast(a));
		tmpnode = new_node(NODE_PIPELINE, *a, b, tmp);
		if (tmpnode == NULL)
		{
			ft_lstdelone(tmp, free_tokennode);
			free_ast(a);
			free_ast(&b);
			return (NULL);
		}
		*a = tmpnode;
	}
	return (*a);
}

/*
 * Parsing a pipeline
 */
t_ast_node	*parse_pipeline(t_list **tokenlist)
{
	t_ast_node	*a;

	if (tokenlist == NULL || *tokenlist == NULL)
		return (NULL);
	a = parse_terminal(tokenlist);
	if (a == NULL)
		return (NULL);
	if (!parse_pipeline_innerloop(&a, tokenlist))
		return (NULL);
	return (a);
}
