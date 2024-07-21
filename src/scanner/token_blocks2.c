/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_blocks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:51:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/21 10:48:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
 * I define a block as the text between two delimiters like {}
 * or "" '' or `` or () etc
 * anything until I get the closing delimiter I specify in delim...
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
		return (scanner_error(data, \
				"syntax error: unexpected end of file"));
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, t_type);
	*i = start;
	free(tmp);
	return (true);
}
