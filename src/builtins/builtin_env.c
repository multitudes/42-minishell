/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:53 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 13:52:56 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
#include "splash_error.h"
#include "debug.h"

/*
Writes contents of environment to standard output.
No options or arguments implemented.
*/
uint8_t	execute_env_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;

	debug("env builtin");
	status = 0;
	if (get_token_lexeme(tokenlist->next))
		status = stderr_and_status("env: too many arguments", 1);
	else
		status = print_env(env_arr);
	return (status);
}
