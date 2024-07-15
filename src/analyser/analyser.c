/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 12:06:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "analyser.h"
#include "scanner.h"
#include <libft.h>
#include "splash_error.h"
#include "utils.h"
#include "builtins.h"

/*
Identify if and which expansion needed
and call the appropriate expansion function.
*/
static void	execute_expansion_by_type(t_data *data, t_list **tokenlist, \
										t_exp_flags *flags)
{
	t_tokentype	type;

	debug("Check and execute expansion by type (token type: %i)", get_token_type(*tokenlist));
	type = get_token_type(*tokenlist);
	if (type == S_QUOTED_STRING)
		expand_single_quotes(get_curr_token(*tokenlist));
	else if (type == QUOTED_STRING)
		expand_double_quotes(data, get_curr_token(*tokenlist));
	else if (type == VAR_EXPANSION || type == DOLLAR \
			|| type == DOLLAR_QUESTION || type == DOLLAR_DIGIT)
		expand_dollar(data, get_curr_token(*tokenlist));
	else if (ft_strchr(get_token_lexeme(*tokenlist), '~') \
			&& type != QUOTE_EXPANDED)
		expand_path(data->env_arr, *tokenlist, flags);
	else if (type == GLOBBING)
		expand_globbing(tokenlist);
}

/*
Goes through tokenlist, expands if expansion types and expansion allowed.
*/
static void	expand_tokenlist(t_data *data, t_ast_node *ast)
{
	t_exp_flags	flags;
	int			count;
	t_list		*tokenlist;

	count = 0;
	tokenlist = ast->tokenlist;
	reset_flags(&flags);
	while (tokenlist)
	{
		if (!get_curr_token(tokenlist))
		{
			data->exit_status = \
				stderr_and_status("system error: missing token", 1);
			return ;
		}
		set_flags(tokenlist, &flags);
		debug("lexeme: %s, type: %i", get_token_lexeme(tokenlist), get_token_type(tokenlist));
		execute_expansion_by_type(data, &tokenlist, &flags);
		if (count == 0)
			ast->tokenlist = tokenlist;
		debug("lexeme after expansion: %s, type: %i", get_token_lexeme(tokenlist), get_token_type(tokenlist));
		if (token_followed_by_space(tokenlist))
			reset_flags(&flags);
		tokenlist = tokenlist->next;
		count++;
	}
}

static bool	separated_token_types(t_list *tokenlist)
{
	t_tokentype	type_1;
	t_tokentype	type_2;

	if (!tokenlist || !tokenlist->next)
		return (true);
	type_1 = get_token_type(tokenlist);
	type_2 = get_token_type(tokenlist->next);
	if (is_redirection_token(type_1) || is_redirection_token(type_2))
		return (true);
	else if (is_heredoc_token(type_1) || is_heredoc_token(type_2))
		return (true);
	return (false);

}

/*
Expansion of nodes containing single quotes, double quotes, variables,
~ / paths, Special Parameters ($?, $0 implemented)
Could be extended to also include other Special Parameters ($!, etc.),
$(..) ${..} $'..' $".."
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	t_list		*tokenlist;

	if (ast == NULL)
		return ;
	tokenlist = ast->tokenlist;
	if (!tokenlist)
		return ;
	debug("First lexeme in tokenlist: -%s-, type: %i", get_token_lexeme(tokenlist), get_token_type(tokenlist));
	expand_tokenlist(data, ast);
	tokenlist = ast->tokenlist;
	// debug("First lexeme in expanded tokenlist: -%s-, type: %i", get_token_lexeme(tokenlist), get_token_type(tokenlist));
	while (tokenlist && tokenlist->next)
	{
		if ((!token_followed_by_space(tokenlist) \
				&& !separated_token_types(tokenlist)) \
			|| (ft_strlen(get_token_lexeme(tokenlist)) == 0 \
				&& get_token_type(tokenlist) != QUOTE_EXPANDED))
			merge_tokens(tokenlist);
		else
			tokenlist = tokenlist->next;
	}
	which_ast_node(ast);
	// debug("First lexeme in expanded and merged tokenlist: -%s-, type: %i, ast-node type: %i", get_token_lexeme(ast->tokenlist), get_token_type(ast->tokenlist),  ast->type);
}
