/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:07:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 11:34:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "minishell.h"

/*
why returning true. I want to tell the scanner that the 
token was successfully recognized so that it stops looking
for other tokens and go back to the while loop with the data error 
in the data struct so it will stop and free the data
*/
bool	scanner_error(t_mini_data *data, char *err_str)
{
	write(2, err_str, ft_strlen(err_str));
	write(2, "\n", 1);
	data->scanner_err_str = err_str;
	data->scanner_error = 2;
	return (true);
}

/*
This function is passed to my ft_lstclear to clear the list contents
Each node's content is a token which 
-has a lexeme which needs to be free
-and then the token itself needs to be freed
*/
void	free_tokennode(void *content)
{
	if (content)
	{
		debug("freeing token node content lexeme %s", ((t_token *)content)->lexeme);
		free(((t_token *)content)->lexeme);
	}
	free(content);
}