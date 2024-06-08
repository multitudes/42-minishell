/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:43:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/08 18:19:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_simple_operator(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "=", false))
		return (add_token(data, i, "=", EQUAL));
	else if (peek(data->input + *i, "&", false))
		return (add_token(data, i, "&", AND_TOK));
	else if (peek(data->input + *i, "^", false))
		return (add_token(data, i, "^", CARET));
	else if (peek(data->input + *i, "%", false))
		return (add_token(data, i, "%", PERCENT));
	else if (peek(data->input + *i, "~/", false))
		return (false);
	else if (peek(data->input + *i, "~", true))
		return (add_token(data, i, "~", TILDE));
	else if (peek(data->input + *i, "$", false))
		return (add_token(data, i, "$", DOLLAR));
	else
		return (false);
}

bool	is_a_control_operator(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "||", false))
		add_token(data, i, "||", OR_IF);
	else if (peek(data->input + *i, "&&", false))
		add_token(data, i, "&&", AND_IF);
	else if (peek(data->input + *i, "|&", false))
		add_token(data, i, "|&", PIPE_AND);
	else if (peek(data->input + *i, ";&", false))
		add_token(data, i, ";&", SEMI_AND);
	else if (peek(data->input + *i, "|", false))
		add_token(data, i, "|", PIPE);
	else
		return (false);
	return (true);
}

bool	is_a_math_op(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "--", false))
		add_token(data, i, "--", MINUS_MINUS);
	else if (peek(data->input + *i, "++", false))
		add_token(data, i, "++", PLUS_PLUS);
	else if (peek(data->input + *i, "-=", false))
		add_token(data, i, "-=", MINUS_EQUAL);
	else if (peek(data->input + *i, "+=", false))
		add_token(data, i, "+=", PLUS_EQUAL);
	else if (peek(data->input + *i, "/=", false))
		add_token(data, i, "/=", SLASH_EQUAL);
	else if (peek(data->input + *i, "*=", false))
		add_token(data, i, "*=", STAR_EQUAL);
	else
		return (false);
	return (true);
}
