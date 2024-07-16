/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tilde2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 00:15:22 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 00:15:26 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "analyser.h"
#include <libft.h>

/*
We dont expand tilde if the next char after the tilde is not a valid path.
Valid chars after tilde are '/', ':', ' ' and '\0'.
*/
static bool	peek_is_valid_path(char c)
{
	if (ft_strchr("/: ", c))
		return (true);
	return (false);
}

/*
Checks if tilde expansion is valid based on next char in lexeme or next token
*/
bool	valid_tilde_expansion(t_list *tokenlist, char *lexeme, int i)
{
	if (!tokenlist->next \
			&& peek_is_valid_path(lexeme[i + 1]))
		return (true);
	else if (token_followed_by_space(tokenlist) \
				&& peek_is_valid_path(lexeme[i + 1]))
		return (true);
	else if (!token_followed_by_space(tokenlist) && tokenlist->next \
				&& peek_is_valid_path(lexeme[i + 1]) \
				&& peek_is_valid_path((get_token_lexeme(tokenlist->next))[0]))
		return (true);
	return (false);
}

bool	tilde_to_be_expanded(char *lexeme, t_exp_flags *flags, \
									t_list *tokenlist, int i)
{
	if (i == 0 && flags->equal_status == 1 \
			&& valid_tilde_expansion(tokenlist, lexeme, i))
		return (true);
	else if (i != 0 && valid_tilde_separator(lexeme[i - 1], flags) \
					&& valid_tilde_expansion(tokenlist, lexeme, i))
		return (true);
	return (false);
}
