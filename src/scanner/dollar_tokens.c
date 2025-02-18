/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:32:51 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 12:49:37 by lbrusa           ###   ########.fr       */
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
	char	dollar_digit_lexeme[3];

	if (peek(data->input + *i, "$((", FUZZY))
		return (add_block_dbl_paren(data, i, "))", EXPR_EXPANSION));
	else if (peek(data->input + *i, "$", FUZZY) && \
	is_digit(*(data->input + *i + 1)))
	{
		dollar_digit_lexeme[0] = '$';
		dollar_digit_lexeme[1] = *(data->input + *i + 1);
		dollar_digit_lexeme[2] = '\0';
		return (add_token(data, i, dollar_digit_lexeme, DOLLAR_DIGIT));
	}
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

bool	is_a_dollar_exp(t_mini_data *data, int *i)
{
	if (is_simple_dollar_exp(data, i))
		return (true);
	else if (is_complex_dollar_exp(data, i))
		return (true);
	else
		return (false);
}
