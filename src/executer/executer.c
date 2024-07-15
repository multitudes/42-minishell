/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 14:33:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "splash.h"
#include "splash_error.h"
#include "builtins.h"
#include "analyser.h"
#include "heredoc.h"
#include "darray.h"
#include "scanner.h"
#include "fd.h"

/*
posix compliant use of the environ variable
*/
extern char	**environ;

/*
 * main loop called from execute_ast
 */
int	execute_loop(t_ast_node *ast, t_data *data)
{
	uint8_t			status;

	while (1)
	{
		if (ast->type == NODE_LIST)
			status = execute_list(ast, data);
		else if (ast->type == NODE_PIPELINE)
			status = execute_pipeline(ast, data);
		else if (contains_redirection(ast->tokenlist) && save_fds(data))
		{
			status = execute_redirection(&ast);
			if (status == 0)
				continue ;
		}
		else if (ast->type == NODE_BUILTIN)
			status = execute_builtin(ast->tokenlist, data);
		else if (ast->type == NODE_COMMAND || ast->type == NODE_TERMINAL)
			status = execute_command(ast->tokenlist, data);
		restore_fds(data);
		break ;
	}
	return (status);
}

/*
 * Traverse the ast and execute the commands node by node 
 * left to right
 */
int	execute_ast(t_ast_node *ast, t_data *data)
{
	int			status;
	t_list		*tokenlist;

	status = 0;
	if (ast == NULL || ast->tokenlist == NULL)
		return (0);
	tokenlist = ast->tokenlist;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
	init_fds(data);
	analyse_expand(ast, data);
	status = execute_loop(ast, data);
	return (status);
}
