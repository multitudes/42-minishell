/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_builtins2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:40:02 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:46:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "libft.h"

/*
 * is_builtin checks if the identifier is a builtin command
 */
bool	is_builtin(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "echo", EXACT))
		add_token(data, start, "echo", BUILTIN);
	else if (peek(identifier, "cd", EXACT))
		add_token(data, start, "cd", BUILTIN);
	else if (peek(identifier, "export", EXACT))
		add_token(data, start, "export", BUILTIN);
	else if (peek(identifier, "unset", EXACT))
		add_token(data, start, "unset", BUILTIN);
	else if (peek(identifier, "env", EXACT))
		add_token(data, start, "env", BUILTIN);
	else if (peek(identifier, "exit", EXACT))
		add_token(data, start, "exit", BUILTIN);
	else if (peek(identifier, "pwd", EXACT))
		add_token(data, start, "pwd", BUILTIN);
	else if (not_implemented_builtin(identifier))
		add_token(data, start, identifier, BUILTIN);
	else
		return (false);
	return (true);
}
