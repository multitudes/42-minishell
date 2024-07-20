/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:52:53 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/20 10:57:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"
#include "scanner.h"
#include "parser.h"
#include "environment.h"
#include "splash_error.h"

/*
Writes contents of environment to standard output.
No options or arguments implemented.
*/
uint8_t	execute_env_builtin(t_darray *env_arr, t_list *tokenlist)
{
	uint8_t	status;

	status = 0;
	if (get_token_lexeme(tokenlist->next))
		status = stderr_and_status("env: too many arguments", 1);
	else
		status = print_env(env_arr);
	return (status);
}
