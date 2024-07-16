/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:52:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:40:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
Returns 'true' if the character is a digit
*/
bool	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}

/*
Returns 'true' if the character is char a letter
*/
bool	is_alpha(const char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/*
part of the check for identifiers
btw This 65-character set, [-._a-zA-Z0-9], 
is referred to in SUSv3 as the portable filename character set.
*/
bool	is_alnum(const char c)
{
	return (is_alpha(c) || is_digit(c) || c == '_' || c == '-' || c == '.');
}

/*
either is alnum including the _ and - or is a / 
I already accept the . and .. and ./ and ~/ and ~+  
as a beginning of a pathname
*/
bool	char_is_in_pathname(const char c)
{
	return (is_alnum(c) || c != '.');
}
