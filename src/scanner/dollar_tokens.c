/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:32:51 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/08 19:09:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

bool	is_simple_dollar_exp(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "$?", FUZZY))
		return (add_token(data, i, "$?", DOLLAR_QUESTION));
	else if (peek(data->input + *i, "$$", FUZZY))
		return (add_token(data, i, "$$", DOLLAR_DOLLAR));
	else if (peek(data->input + *i, "$*", FUZZY))
		return (add_token(data, i, "$*", DOLLAR_STAR));
	else if (peek(data->input + *i, "$@", FUZZY))
		return (add_token(data, i, "$@", DOLLAR_AT));
	else if (peek(data->input + *i, "$#", FUZZY))
		return (add_token(data, i, "$#", DOLLAR_HASH));
	else if (peek(data->input + *i, "$!", FUZZY))
		return (add_token(data, i, "$!", DOLLAR_BANG));
	else if (peek(data->input + *i, "$-", FUZZY))
		return (add_token(data, i, "$-", DOLLAR_HYPHEN));
	else if (peek(data->input + *i, "$~", FUZZY))
		return (add_token(data, i, "$~", DOLLAR_TILDE));
	else
		return (false);
}

bool	is_complex_dollar_exp(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "$((", FUZZY))
		return (add_block_dbl_paren(data, i, "))", EXPR_EXPANSION));
	else if (peek(data->input + *i, "$", FUZZY) && \
	is_digit(*(data->input + *i + 1)))
		return (proc_token_off_1(data, i, is_digit, \
		VAR_EXPANSION));
	else if (peek(data->input + *i, "${", FUZZY))
		return (add_tokenblock(data, i, '}', VAR_EXPANSION));
	else if (peek(data->input + *i, "$", FUZZY) && is_alnum(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alnum, VAR_EXPANSION));
	else if (peek(data->input + *i, "$(", FUZZY))
		return (add_tokenblock(data, i, ')', COM_EXPANSION));
	else
		return (false);
}

/*
    DOLLAR_QUESTION, // '$?'  The special parameter ‘?’ is used to get the 
	exit status of the last command.
    DOLLAR_DOLLAR, // '$$' ‘$’ is used to get the process ID of the shell.
    DOLLAR_STAR, // '$*' ‘*’ is used to get all the positional parameters.
    DOLLAR_AT, // '$@'  ‘@’ is used to get all the positional parameters, 
	except for the zeroth positional parameter.
    DOLLAR_HASH, // '$#'  ‘#’ is used to get the number of positional 
	parameters.
    DOLLAR_BANG, // '$!'  ‘!’ is used to get the process ID of the last 
	background command.
	DOLLAR_HYPHEN, // '$-' used to get the current options set for the shell.	 
	VAR_EXPANSION, // '$0' ‘0’ is used to get the name of the shell or script.
	Parameter names in bash can only contain alphanumeric 
	characters or underscores, and must start with a letter or underscore.
*/
bool	is_a_dollar_exp(t_mini_data *data, int *i)
{
	if (is_simple_dollar_exp(data, i))
		return (true);
	else if (is_complex_dollar_exp(data, i))
		return (true);
	else
		return (false);
}
