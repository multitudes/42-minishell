/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:01:41 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/16 19:01:44 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "analyser.h"
#include <libft.h>

static void	remove_backslash(char **str)
{
	char	*temp;

	if (*str && (*str)[0] != '\\')
		return ;
	else if (*str && (*str)[0] == '\\' \
			&& ((*str)[1] == '\0' || (*str)[1] == '$'))
	{
		temp = ft_strdup((*str) + 1);
		free (*str);
		*str = temp;
	}
}

char	*expand_heredoc(bool expand, t_data *data, char *line)
{
	char	*new_line;

	if (line && expand == true && ft_strchr(line, '$') && line[0] != '\\')
	{
		new_line = replace_dollar_vars(data, line);
		free(line);
	}
	else
		new_line = line;
	remove_backslash(&new_line);
	return (new_line);
}
