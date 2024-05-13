/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:56:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:58:49 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
History expansion - (not implemented)!
!!: Re-run the previous command. 
This is useful if you forgot to use sudo for a command that 
requires it. You can simply type sudo !! to re-run the previous 
command with sudo.

!n: Re-run the nth command in your history. 
For example, !100 would re-run the 100th command.

!-n: Re-run the command n lines back. 
For example, !-2 would re-run the second most recent command.

!string: Re-run the most recent command that 
starts with string. For example, !ls would re-run the most recent command 
that starts with ls.

!?string?: Re-run the most recent command 
that contains string anywhere. For example, !?txt? would re-run the most 
recent command that includes txt.
BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA
*/
bool	is_a_hist_expansion(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "!!", false))
		return (process_token(data, i, is_not_delimiter, BANG_BANG));
	else if (peek(data->input + *i, "!", false) && is_digit(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_digit, BANG_DIGIT));
	else if (peek(data->input + *i, "!-", false) && is_digit(*(data->input \
	+ *i + 2)))
		return (proc_tok_off_2(data, i, is_digit, BANG_HYPHEN_DIGIT));
	else if (peek(data->input + *i, "!", false) && is_alpha(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alpha, BANG_ALPHA));
	else if (peek(data->input + *i, "!?", false))
		return (process_token(data, i, is_not_delimiter, BANG_QUESTION_ALPHA));
	else if (peek(data->input + *i, "!#", false))
		return (add_token(data, i, "!#", BANG_HASH));
	return (false);
}
