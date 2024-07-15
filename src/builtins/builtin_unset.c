/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:53:10 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:53:14 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
#include "builtins.h"
#include "splash_error.h"
#include "debug.h"

/*
Builtin function to unset variables.
Checks for specific read-only variables, which cannot be unset.
*/
uint8_t	execute_unset_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;
	char	*lexeme;

	status = 0;
	debug("unset builtin");
	tokenlist = tokenlist->next;
	if (!tokenlist)
		return (0);
	while (tokenlist)
	{
		lexeme = get_token_lexeme(tokenlist);
		if (ft_strchr(lexeme, '='))
			;
		else if (read_only_variable(lexeme))
			status = stderr_and_status3("unset: ", lexeme, \
									": cannot unset: readonly variable", 1);
		else if (lexeme != NULL && ft_strlen(lexeme) != 0)
			status = delete_env_entry(env_arr, lexeme);
		else
			status = stderr_and_status("unset: error", 1);
		tokenlist = tokenlist->next;
	}
	return (status);
}
