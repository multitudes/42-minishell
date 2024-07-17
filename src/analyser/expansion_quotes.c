/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:01:22 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 13:49:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "analyser.h"
#include "scanner.h"
#include "libft.h"

void	expand_single_quotes(t_token *token)
{
	char	*lexeme;

	if (token && token->lexeme)
	{
		lexeme = ft_strtrim(token->lexeme, "'"); 
		free(token->lexeme);
		token->lexeme = lexeme;
		token->type = QUOTE_EXPANDED;
	}
}

/*
TODO: this may  not behave well, when we have strings like "\"djklfjsdl\""
or ""dkldfj" as escape characters and unclosed quotes are not handled ;
*/
void	expand_double_quotes(t_data *data, t_token *token)
{
	char	*unquoted_lexeme;

	unquoted_lexeme = NULL;
	if (!token)
		return ;
	unquoted_lexeme = ft_strtrim(token->lexeme, "\"");
	free(token->lexeme);
	if (!ft_strchr(unquoted_lexeme, '$') || single_dollar(unquoted_lexeme))
		token->lexeme = ft_strdup(unquoted_lexeme);
	else if (ft_strcmp(unquoted_lexeme, "$0") == 0)
		token->lexeme = ft_strdup("splash");
	else
		token->lexeme = replace_dollar_vars(data, unquoted_lexeme);
	free(unquoted_lexeme);
	token->type = QUOTE_EXPANDED;
}
