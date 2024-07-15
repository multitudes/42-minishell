/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:33 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:52:37 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "parser.h"
#include "debug.h"
#include "builtins.h"
#include "utils.h"

/*
Executes builtin "echo" function with and without '-n' option
which suppresses the trailing newline.
*/
uint8_t	execute_echo_builtin(t_list *tokenlist)
{
	uint8_t	status;
	int		new_line;

	debug("echo builtin");
	status = 0;
	new_line = 1;
	tokenlist = tokenlist->next;
	while (tokenlist && get_token_type(tokenlist) == FLAGS \
					&& allowed_flags(get_token_lexeme(tokenlist), "n"))
	{
		new_line = 0;
		tokenlist = tokenlist->next;
	}
	while (tokenlist)
	{
		write_data(1, get_token_lexeme(tokenlist), &status);
		if (tokenlist->next != NULL && token_followed_by_space(tokenlist))
			write_data(1, " ", &status);
		tokenlist = tokenlist->next;
	}
	if (new_line)
		write_data(1, "\n", &status);
	return (status);
}
