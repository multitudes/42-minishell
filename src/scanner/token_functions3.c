/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:24:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:30:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "debug.h"
#include "libft.h"

/*
 * create a lexeme for flag in this conf -[a-zA-Z]
 * else if (peek((data->input + i), "-", FUZZY) && is_alpha(data->input[i + 1]))
 */
bool	is_a_flag(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	if (peek(data->input + *i, "-", FUZZY) && is_alpha(data->input[*i + 1]))
	{
		start = (*i)++;
		while (data->input[*i] && is_alpha(data->input[*i]))
			advance(i);
		tmp = ft_substr(data->input, start, *i - start);
		add_token(data, &start, tmp, FLAGS);
		*i = start;
		free(tmp);
		return (true);
	}
	return (false);
}

/*
 * 
 */
bool	is_some_semicolons(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ";;&", FUZZY))
		add_token(data, i, ";;&", DSEMI_AND);
	else if (peek(data->input + *i, ";;", FUZZY))
		add_token(data, i, ";;", DSEMI);
	else if (peek(data->input + *i, ";", FUZZY))
		add_token(data, i, ";", SEMI);
	else if (peek(data->input + *i, "!", EXACT))
		add_token(data, i, "!", BANG);
	else
		return (false);
	return (true);
}

/*
 */
bool	is_a_string_thing(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	start = *i;
	while (not_a_delimiting_char(data->input[*i]))
		advance(i);
	if (*i == start)
		return (false);
	tmp = ft_substr(data->input, start, *i - start);
	if ((data->input[*i] == '<' || data->input[*i] == '>') \
	&& is_io_number(tmp))
		add_token(data, &start, tmp, IO_NUMBER);
	else if (!is_reserved(data, tmp, &start) && !is_builtin(data, \
	tmp, &start) && !is_true_false(data, tmp, &start) && \
	!is_a_pathname_or_num(data, tmp, &start) && \
	!is_a_globbing(data, tmp, &start))
		add_token(data, &start, tmp, WORD);
	free(tmp);
	return (true);
}
