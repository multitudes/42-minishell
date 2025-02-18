/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:52:42 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 12:53:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

bool	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

bool	is_alpha(const char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * part of the check for identifiers
 * btw This 65-character set, [-._a-zA-Z0-9], 
 * is referred to in SUSv3 as the portable filename character set.
 */
bool	is_alnum(const char c)
{
	return (is_alpha(c) || is_digit(c) || c == '_' || c == '-' || c == '.');
}

bool	char_is_in_pathname(const char c)
{
	return (is_alnum(c) || c != '.');
}
