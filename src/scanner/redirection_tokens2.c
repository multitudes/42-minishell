/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_tokens2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:47:55 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:47:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "parser.h"

/*
 */
bool	is_heredoc_token(t_tokentype tokentype)
{
	if (tokentype == DLESS || tokentype == DLESSDASH || \
			tokentype == HEREDOC_FILE)
		return (true);
	return (false);
}

/*
 * Check if assigned type is a valid heredoc delimiter, in which case
 * the type is changed to DLESS_DELIM before returning result of check.
 * ADD MORE VALID TOKENTYPES TODO
 */
bool	is_heredoc_delim(t_list *tokenlist)
{
	t_tokentype	tokentype;
	bool		heredoc_delim;

	if (!tokenlist)
		return (false);
	tokentype = get_token_type(tokenlist);
	if (tokentype == DLESS_DELIM || tokentype == DLESSDASH)
		return (true);
	else if (tokentype == TILDE)
		heredoc_delim = true;
	else if (tokentype == VAR_EXPANSION)
		heredoc_delim = true;
	else
		heredoc_delim = false;
	((t_token *)(tokenlist->content))->type = DLESS_DELIM;
	return (heredoc_delim);
}
