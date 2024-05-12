/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:07:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:14:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
why returning true. I want to tell the scanner that the 
token was successfully recognized so that it stops looking
for other tokens and go back to the while loop with the data error 
in the data struct so it will stop and free the data
*/
bool	scanner_error(t_mini_data *data, char *err_str)
{
	debug("error: %s\n", err_str);
	data->scanner_err_str = err_str;
	data->scanner_error = 1;
	return (true);
}

/*
I have a linked lists of nodes. each node's content is a token
I need to pass this function to my ft_lstclear to clear the list contents
and because the content is a token which 
-has a lexeme which needs to be free
-and then the token itself needs to be freed
*/
void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	free(token->lexeme);
	free(token);
}
