/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:57:41 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/09 13:05:51 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
if contains a slash or starts with a dot or starts with a ./ ../ ~/ ~+
*/
bool	str_is_pathname(const char *str)
{
	if (((ft_strchr(str, '/') && (!ft_strchr(str, '~'))) || peek(str, ".", false) || peek(str, "./", \
	false) || peek(str, "../", false) || peek(str, "~/", false) || \
	peek(str, "~+/", false) || peek(str, "~+", true) || peek(str, "~-/", false) || peek(str, "~-", true)) && !ft_strchr(str, '*'))
	{
		while (*str)
		{
			if (!is_in_pathname(*str))
				return (false);
			str++;
		}
		return (true);
	}
	return (false);
}

/*
Recognizing a globbing wanna be expansion, any string with the letter *
it will be for the bonus eventually!
*/
bool	is_a_globbing(t_mini_data *data, const char *tmp, int *start)
{
	if (ft_strchr(tmp, '*'))
		add_token(data, start, tmp, GLOBBING);
	else
		return (false);
	return (true);
}

bool	is_a_pathname_or_num(t_mini_data *data, const char *tmp, int *start)
{
	if (str_is_pathname(tmp))
		add_token(data, start, tmp, PATHNAME);
	else if (str_is_number(tmp))
		add_token(data, start, tmp, NUMBER);
	else
		return (false);
	return (true);
}

/*
*/
bool	str_is_alphanum(const char *str)
{
	while (*str)
		if (!is_alnum(*(str++)))
			return (false);
	return (true);
}

/*
printable chars include a space. This is for the identifiers
*/
int	isprint_no_space(const char *str)
{
	while (*(str))
	{
		if (is_space(*str) || !isprint(*str))
			return (0);
		str++;
	}
	return (1);
}

/*
just for style, it removes ugly pointer arythmetics 
from my code
*/
void	advance(int *i)
{
	(*i)++;
}
