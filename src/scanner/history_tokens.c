/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:56:08 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 12:49:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

bool	is_a_hist_expansion(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "!!", FUZZY))
		return (process_token(data, i, is_not_delimiter, BANG_BANG));
	else if (peek(data->input + *i, "!", FUZZY) && is_digit(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_digit, BANG_DIGIT));
	else if (peek(data->input + *i, "!-", FUZZY) && is_digit(*(data->input \
	+ *i + 2)))
		return (proc_tok_off_2(data, i, is_digit, BANG_HYPHEN_DIGIT));
	else if (peek(data->input + *i, "!", FUZZY) && is_alpha(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alpha, BANG_ALPHA));
	else if (peek(data->input + *i, "!?", FUZZY))
		return (process_token(data, i, is_not_delimiter, BANG_QUESTION_ALPHA));
	else if (peek(data->input + *i, "!#", FUZZY))
		return (add_token(data, i, "!#", BANG_HASH));
	return (false);
}
