/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:02:17 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 13:49:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyser.h"
#include "scanner.h"
#include "utils.h"
#include "libft.h"

/*
If there is a : or a first equal sign we expand.
If we are already past the first equal sign we dont expand if 
we have more equal signs in between.
*/
bool	valid_tilde_separator(char sep, t_exp_flags *flags)
{
	if (ft_strchr(": ", sep))
		return (true);
	else if (sep == '=' && flags->equal_status == 1 && flags->valid_key)
		return (true);
	return (false);
}

static char	*expand_tilde(char *lexeme, char *home, t_exp_flags *flags, int i)
{
	char	*front;
	char	*back;
	char	*new_lexeme;

	back = ft_strdup(lexeme + i + 1);
	if (i == 0)
		new_lexeme = ft_strjoin(home, back);
	else
	{
		front = ft_strndup(lexeme, i);
		if (flags->equal_status == 1)
			flags->equal_status = 2;
		new_lexeme = ft_strjoin3(front, home, back);
		free(front);
	}
	free(lexeme);
	free(back);
	return (new_lexeme);
}

/*
Replaces occurances of '~' in a string, if conditions for ~-expansion are met.
*/
static char	*replace_tilde_in_lexeme(t_list *tokenlist, char *home, \
									t_exp_flags *flags)
{
	char	*lexeme;
	int		i;

	i = 0;
	lexeme = ft_strdup(get_token_lexeme(tokenlist));
	if (flags->equal_status == 1 && ft_strchr(lexeme, '='))
		i = ft_strchr(lexeme, '=') - lexeme + 1;
	while (lexeme && lexeme[i])
	{
		if (lexeme[i] == '~' \
			&& tilde_to_be_expanded(lexeme, flags, tokenlist, i))
		{
			lexeme = expand_tilde(lexeme, home, flags, i);
			i = i + ft_strlen(home) - 1;
		}
		if (lexeme[i] == '=' && flags->equal_status == 1)
			flags->equal_status = 2;
		i++;
	}
	return (lexeme);
}

/*
Expands "~" in pathnames
*/
void	expand_path(t_darray *env_arr, t_list *tokenlist, t_exp_flags *flags)
{
	char	*lexeme;
	char	*home;
	t_token	*token;

	token = get_curr_token(tokenlist);
	if (!token)
		return ;
	home = get_home(env_arr);
	if (token->type == TILDE && flags->equal_status == 1
		&& valid_tilde_expansion(tokenlist, get_token_lexeme(tokenlist), 0) \
		&& flags->valid_key)
		lexeme = ft_strdup(home);
	else
		lexeme = replace_tilde_in_lexeme(tokenlist, home, flags);
	free(home);
	token->type = WORD;
	free(token->lexeme);
	token->lexeme = lexeme;
}
