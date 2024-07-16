/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_blocks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:51:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:39:49 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
params:
- str: the string to check
- i: the current index in the string
- start: the index of the opening parenthesis
returns true:
- check for empty string in parenthesis
- parentheses are not valid if they are followed or preceded 
by characters other than && || | or spaces
*/ 
bool	is_empty_parenthesis(const char *str, int i, int start)
{
	int	j; 

	j = start + 1;
	if ((i - start) == 2)
		return (true);
	while (j <= (i - 2))
	{
		if (!is_space(str[j]) && (str[j] != '(') && (str[j] != ')'))
			return (false);
		j++;
	}
	return (true);
}

/*
I define a block as the text between two delimiters like {}
or "" '' or `` or () etc
anything until I get the closing delimiter I specify in delim...
*/
bool	add_parenthesisblock(t_mini_data *data, int *i, char delim, int t_type)
{
	int		start;
	char	*tmp;
	int		count;

	start = (*i)++;
	count = 1;
	while (data->input[*i] && count > 0)
	{
		if (data->input[*i] == '(')
			count++;
		if (data->input[*i] == delim)
			count--;
		advance(i);
	}
	if (count != 0)
		return (scanner_error(data, "minishell: syntax error: \
		unexpected end of file"));
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, t_type);
	*i = start;
	free(tmp);
	return (true);
}
