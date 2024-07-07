/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_tokens.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:47:55 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/07 17:13:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

bool	add_here_and_delim(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	add_token(data, i, "<<", DLESS);
	while (data->input[*i] && is_space(data->input[*i]))
		advance(i);
	if (data->input[*i] == '\0' || is_delimiter(data->input[*i]))
		return (scanner_error(data, "minishell: syntax error"));
	start = *i;
	while ((data->input + *i) && !is_delimiter(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	*i = start;
	add_token(data, i, tmp, DLESS_DELIM);
	free(tmp);
	return (true);
}

bool	is_a_simple_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ">|", FUZZY))
		return (add_token(data, i, ">|", CLOBBER));
	else if (peek(data->input + *i, "<<-", FUZZY))
		return (add_token(data, i, "<<-", DLESSDASH));
	else if (peek(data->input + *i, "&>>", FUZZY))
		return (add_token(data, i, "&>>", REDIRECT_BOTH_APP));
	else if (peek(data->input + *i, "&>", FUZZY))
		return (add_token(data, i, "&>", REDIRECT_BOTH));
	else if (peek(data->input + *i, "<&", FUZZY))
		return (add_token(data, i, "<&", LESSAND));
	else if (peek(data->input + *i, ">&", FUZZY))
		return (add_token(data, i, ">&", GREATAND));
	else if (peek(data->input + *i, "<>", FUZZY))
		return (add_token(data, i, "<>", LESSGREAT));
	else if (peek(data->input + *i, ">>", FUZZY))
		return (add_token(data, i, ">>", DGREAT));
	else
		return (false);
	return (true);
}

bool	is_a_aggregate_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "<<", FUZZY))
		return (add_here_and_delim(data, i));
	else if (peek(data->input + *i, ">=", FUZZY))
		return (add_token(data, i, ">=", GREATER_EQUAL));
	else if (peek(data->input + *i, "<=", FUZZY))
		return (add_token(data, i, "<=", LESS_EQUAL));
	else if (peek(data->input + *i, "!=", FUZZY))
		return (add_token(data, i, "!=", BANG_EQUAL));
	else if (peek(data->input + *i, ">", FUZZY))
		return (add_token(data, i, ">", REDIRECT_OUT));
	else if (peek(data->input + *i, "<", FUZZY))
		return (add_token(data, i, "<", REDIRECT_IN));
	else if (peek(data->input + *i, ",", FUZZY))
		return (add_token(data, i, ",", COMMA));
	else if (peek(data->input + *i, "-", EXACT))
		return (add_token(data, i, "-", MINUS));
	else
		return (false);
}

bool	is_a_redirection(t_mini_data *data, int *i)
{
	if (is_a_simple_redirection(data, i))
		return (true);
	else if (is_a_aggregate_redirection(data, i))
		return (true);
	else
		return (false);
}
