/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:07:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/17 09:07:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "splash.h"
#include "utils.h"
#include "splash_error.h"

/*
 * again for overview. This is the data struct
 * living inside the scanner. It holds 
 * - the input
 * - error flag to stop the loop
 * - error string to print the error
 * - the token list to return to the parser or free
 * 
 * struct s_mini_data 
 * {
 * 	const char	*input;
 * 	t_list		*tokenlist;
 * 	bool		scanner_error;
 * 	char		*scanner_err_str;
 * };
 * typedef struct s_mini_data t_mini_data;
 */
int	init_scanner_data(t_mini_data *data, const char *input)
{
	data->input = input;
	data->tokenlist = NULL;
	data->scanner_error = false;
	data->scanner_err_str = NULL;
	return (1);
}

/*
 * why returning true. I want to tell the scanner that the 
 * token was successfully recognized so that it stops looking
 * for other tokens and go back to the while loop with the data error 
 * in the data struct so it will stop and free the data
 */
bool	scanner_error(t_mini_data *data, char *err_str)
{
	stderr_and_status(err_str, 0);
	data->scanner_err_str = err_str;
	data->scanner_error = 2;
	return (true);
}

/*
 * This function is passed to my ft_lstclear to clear the list contents
 * Each node's content is a token which 
 * -has a lexeme which needs to be free
 * -and then the token itself needs to be freed
 */
void	free_tokennode(void *content)
{
	if (content)
		free(((t_token *)content)->lexeme);
	free(content);
}
