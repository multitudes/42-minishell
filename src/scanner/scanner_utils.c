/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:48:36 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 12:52:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

bool	is_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' \
	|| c == '\r');
}

/**
 * Returns 'true' if the character is a delimiter. Because an identifier
 * can be unofficially almost anything, the delimiter is a character that
 * should not be part of an identifier. Recommended char for identifiers
 * are underscore and alphanumeric characters.
 * but from the bash manual:
 * A metacharacter is a space, tab, newline, 
 * or one of the following characters: 
 * ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
 * I use this function to understand where to break the string
 * into tokens
 */
bool	is_delimiter(const char ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '|' || \
	ch == '&' || ch == ';' || ch == '(' || ch == ')' || \
	ch == '>' || ch == '<' || ch == '\0')
		return (true);
	return (false);
}

/**
 * I need this because I want to use the function pointer
 * passing the negated condition !is_delimiter as a func pointer 
 * is not possible unless I create a new function  
 */
bool	is_not_delimiter(char c)
{
	return (!is_delimiter(c));
}

/**
 * officially it is recommended to use only the 65 characters [-._a-zA-Z0-9]
 * for files and directories ...
 */
bool	filename_delimiter(const char ch)
{
	if (is_space(ch) || ch == ';' || ch == '>' || \
	ch == '<' || ch == '(' || ch == ')' || \
	ch == '|' || ch == '&' || ch == '$' || ch == '`' || \
	ch == '"' || ch == '\'' || ch == '\0')
		return (true);
	return (false);
}
