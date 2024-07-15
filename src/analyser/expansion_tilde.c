/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:02:17 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:15:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyser.h"
#include "scanner.h"
#include "utils.h"
#include <libft.h>

/*
We dont expand tilde if the next char after the tilde is not a valid path.
*/
bool	peek_is_valid_path(char c)
{
	if (ft_strchr("/:", c) || c == '\0')
		return (true);
	return (false);
}

/*
If there is a : or a first equal sign we expand.
If we are already past the first equal sign we dont expand if 
we have more equal signs in between.
*/
bool	valid_tilde_separator(char sep, int equal_status)
{
	if (ft_strchr(": ", sep))
		return (true);
	else if (sep == '=' && equal_status == 1)
		return (true);
	return (false);
}

/*
Replaces occurances of '~' in a string, if conditions for ~-expansion are met.
*/
char	*replace_tilde_in_str(t_list *tokenlist, char *lexeme, \
                                char *home, t_exp_flags *flags)
{
	char	*new_lexeme;
	int		i;
	char	*pos;
	char	*front;
	char	*back;
	char	*temp;

	i = 0;
	new_lexeme = ft_strdup(lexeme);
	if (flags->equal_status == 1)
	{
		pos = ft_strchr(new_lexeme, '=');
		if (!pos)
			pos = new_lexeme;
		else
			i = pos - new_lexeme + 1;
	}
	else
		pos = new_lexeme;
	while (new_lexeme && new_lexeme[i])
	{
		if (new_lexeme[i] == '~' && i == 0 && peek_is_valid_path(new_lexeme[i + 1]) && (flags->equal_status == 1) && valid_tilde_expansion(tokenlist, i))
		{
			back = ft_strdup(new_lexeme + 1);
			temp = new_lexeme;
			new_lexeme = ft_strjoin(home, back);
			free(back);
			free(temp);
			i = ft_strlen(home) - 1;
		}
		else if (new_lexeme[i] == '~' && peek_is_valid_path(new_lexeme[i + 1]) && (i != 0 && valid_tilde_separator(new_lexeme[i - 1], flags->equal_status)) && valid_tilde_expansion(tokenlist, i))
		{
			front = ft_strndup(new_lexeme, i);
			back = ft_strdup(new_lexeme + i + 1);
			if (flags->equal_status == 1)
				flags->equal_status = 2;
			temp = new_lexeme;
			new_lexeme = ft_strjoin3(front, home, back);
			free(front);
			free(back);
			free(temp);
			i = i + ft_strlen(home) - 1;
		}
		if (new_lexeme[i] == '=' && flags->equal_status == 1)
			flags->equal_status = 2;
		i++;
	}
	return (new_lexeme);
}

/*
Checks if tilde expansion is valid based on next char in lexeme or next token
*/
bool	valid_tilde_expansion(t_list *tokenlist, int index)
{
	if (!tokenlist->next)
		return (true);
	else if (token_followed_by_space(tokenlist))
		return (true);
	else if (!token_followed_by_space(tokenlist) \
				&& peek_is_valid_path(get_token_lexeme(tokenlist)[index + 1]) \
				&& peek_is_valid_path((get_token_lexeme(tokenlist->next))[0]))
		return (true);
	return (false);
}
