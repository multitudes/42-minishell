/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:11:00 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 14:34:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
again for overview. This is the data struct
living inside the scanner. It holds 
- the input
- error flag to stop the loop
- error string to print the error
- the token list to return to the parser or free

struct s_mini_data 
{
	const char	*input;
	t_list		*token_list;
	bool		scanner_error;
	char		*scanner_err_str;
};
typedef struct s_mini_data t_mini_data;
*/
int	init_scanner_data(t_mini_data **data, const char *input)
{
	*data = (t_mini_data *)malloc(sizeof(t_mini_data));
	if (*data == NULL)
	{
		debug("error: scanner malloc failed\n");
		return (0);
	}
	(*data)->input = input;
	(*data)->token_list = NULL;
	(*data)->scanner_error = false;
	(*data)->scanner_err_str = NULL;
	return (1);
}

/*
I free the minidata struct used in the scanner but return 
the pointer to what I need, the tokenlist
*/
t_list	*free_scanner_data(t_mini_data **data)
{
	t_list	*tokenlist;

	tokenlist = (*data)->token_list;
	if (data == NULL || *data == NULL)
		return (NULL);
	// free((void *)(*data)->input);
	free(*data);
	*data = NULL;
	return (tokenlist);
}
