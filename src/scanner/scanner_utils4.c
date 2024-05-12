/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:03:58 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:04:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * is_true_false checks if the identifier is a boolean value
 * like in bash true and false are not builtins but reserved words
 */
bool	is_true_false(t_mini_data *data, char *str, int *start)
{
	if (peek(str, "true", true))
		add_token(data, start, "true", TRUETOK);
	else if (peek(str, "false", true))
		add_token(data, start, "false", FALSETOK);
	else
		return (false);
	return (true);
}

void	print_token_list(t_list *token_list)
{
	t_list	*current;
	t_token	*token;

	current = token_list;
	while (current != NULL)
	{
		token = (t_token *)(current)->content;
		printf("token: %s", token->lexeme);
		current = current->next;
	}
	return ;
}
