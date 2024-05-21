/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_blocks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:51:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/21 13:54:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
I define a block as the text between two delimiters like {}
or "" '' or `` or () etc
anything until I get the closing delimiter I specify in delim...
*/
bool	add_parenthesisblock(t_mini_data *data, int *i, char delim, int t_type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (data->input[*i] && data->input[*i] != delim)
		advance(i);
	if (data->input[*i] == '\0')
		return (scanner_error(data, "error: unclosed expression"));
	tmp = ft_substr(data->input, start, *i - start + 1);
	add_token(data, &start, tmp, t_type);
	*i = start;
	free(tmp);
	return (true);
}
