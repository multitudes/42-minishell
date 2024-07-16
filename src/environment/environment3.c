/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:44:30 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 13:45:24 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * Returns the key in a variable string.
 * Memory for key is malloced and needs to be freed.
 */
char	*get_var_key(const char *variable)
{
	char	*position;
	char	*key;

	key = NULL;
	position = ft_strchr(variable, '=');
	if (position)
		key = ft_substr(variable, 0, position - variable);
	else
		key = ft_strdup(variable);
	return (key);
}

/*
 * Returns the value in a variable string.
 * Memory for value is malloced and needs to be freed.
 */
char	*get_var_value(const char *variable)
{
	char	*value;
	char	*position;

	value = NULL;
	position = ft_strchr(variable, '=');
	if (position)
		value = ft_substr(variable, position - variable + 1, \
				ft_strlen(position + 1));
	else
		value = NULL;
	return (value);
}
