/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:17:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 14:55:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

/*
 * function called in parse list inner loop
 * to save lines of code- returns the b node or NULL
 * I dont need to return a node but I need to return a pointer
 * to catch the NULL return 
 */
void	*get_b_node(t_ast_node **b, t_list **tokenlist, t_list *tmp)
{
	if (get_token_type(tmp) != EXPRESSION && \
			!movetonexttoken_andbreak(tokenlist))
	{
		ft_lstdelone(tmp, free_tokennode);
		return (NULL);
	}
	*b = parse_pipeline(tokenlist);
	if (*b == NULL)
	{
		ft_lstdelone(tmp, free_tokennode);
		return (NULL);
	}
	return (*b);
}

/*
 * function called in parse list to parse the inner loop
 *
 */
void	*parse_list_innerloop(t_ast_node **a, t_list **tokenlist)
{
	t_list		*tmp;
	t_ast_node	*b;

	while (*tokenlist)
	{
		break_list(tokenlist);
		tmp = *tokenlist;
		if (get_token_type(tmp) == AND_IF || get_token_type(tmp) == OR_IF || \
		get_token_type(tmp) == EXPRESSION)
		{
			if (!get_b_node(&b, tokenlist, tmp))
				return (free_ast(a));
			*a = new_node(NODE_LIST, *a, b, tmp);
			if (*a == NULL)
				return (free_ast(a));
		}
		else if (*tokenlist)
			*tokenlist = (*tokenlist)->next;
	}
	return (*a);
}

/*
 * Parsing sequence of pipelines separated by operators "&&", "||"
 */
t_ast_node	*parse_list(t_list **tokenlist)
{
	t_ast_node	*a;

	if (tokenlist == NULL || *tokenlist == NULL)
		return (NULL);
	a = parse_pipeline(tokenlist);
	if (a == NULL)
		return (NULL);
	if (!parse_list_innerloop(&a, tokenlist))
		return (NULL);
	return (a);
}
