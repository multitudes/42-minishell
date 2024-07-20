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

static bool	valid_heredoc_delim(char *delim_lexeme)
{
	char	*pos_1;
	char	*pos_2;
	char	*pos_3;
	char	*pos_4;
	bool	result;

	result = true;
	if (!delim_lexeme || ft_strlen(delim_lexeme) == 0)
		return (false);
	pos_1 = ft_strchr(delim_lexeme, '(');
	pos_2 = ft_strchr(delim_lexeme, ')');
	pos_3 = ft_strchr(delim_lexeme, '\"');
	pos_4 = ft_strchr(delim_lexeme, '\'');
	if (pos_1 && ((pos_3 && pos_3 < pos_1) || (pos_4 && pos_4 < pos_1)))
		result = true;
	else if (pos_2 && ((pos_3 && pos_3 < pos_2) || (pos_4 && pos_4 < pos_2)))
		result = true;
	else if (pos_1 || pos_2)
		return (false);
	return (result);
}

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
	if (!valid_heredoc_delim(get_token_lexeme(tokenlist)))
		return (false);
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
