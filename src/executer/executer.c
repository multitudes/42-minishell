/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 09:24:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "minishell.h"
#include "error.h"
#include "builtins.h"
#include "analyser.h"

/*
posix compliant use of the environ variable but wecan discuss this
*/
extern char	**environ;

/*
Checks if tokenlist contains a redirection token.
*/
static bool	contains_redirection(t_list *tokenlist)
{
	debug("contains redirection (check)");
	t_tokentype	tokentype;
	while (tokenlist)
	{
		tokentype = get_token_type(tokenlist);
		if (tokentype == REDIRECT_IN || tokentype == REDIRECT_OUT)
			return (true);
		else if (tokentype == REDIRECT_BOTH || tokentype == REDIRECT_BOTH_APP)
			return (true);
		else if (tokentype == REDIRECT_OUT_APP)
			return (true);
		tokenlist = tokenlist->next;
	}
	return (false);
}

int	execute_pipeline(t_ast_node *ast, t_data *data)
{
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(data->pipe_fd) == -1)
		return (status_and_perror("pipe error", 1));
	pid1 = fork();
	if (pid1 == -1)
		return (status_and_perror("fork 1 error", 1));
	else if (pid1 == 0)
		handle_first_child_process(data, ast);
	pid2 = fork();
	if (pid2 == -1)
		return (status_and_perror("fork 2 failed", 1));
	else if (pid2 == 0)
		handle_second_child_process(data, ast);
	if (close(data->pipe_fd[0]) == -1)
		return (status_and_perror("close fd 5", 1));
	if (close(data->pipe_fd[1]) == -1)
		return (status_and_perror("close fd 6", 1));
	return (get_status_of_children(pid1, pid2));
}

/*
Traverse the ast and execute the commands node by node 
left to right
*/
int	execute_ast(t_ast_node *ast, t_data *data)
{
	int			status;
	t_list		*tokenlist;
	t_nodetype	astnodetype;

	status = 0;
	if (ast == NULL || ast->token_list == NULL)
		return (0);
	debug("\nexecute:");
	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
	debug("ast node type: %d ", ast->type);
	analyse_expand(ast, data);
	astnodetype = ast->type;
	while (1)
	{
		if (astnodetype == NODE_LIST)
			status = execute_list(ast, data);
		else if (astnodetype == NODE_PIPELINE)
			status = execute_pipeline(ast, data);
		else if (contains_redirection(ast->token_list))
		{
			debug("Token contains redirection type!");
			status = execute_redirection(&ast);
			continue ;
		}
		// else if (astnodetype == NODE_REDIRECTION)
		// 	status = execute_redirection(ast, data);
		else if (astnodetype == NODE_BUILTIN)
			status = execute_builtin(ast->token_list, data);
		else if (astnodetype == NODE_COMMAND)
			status = execute_command(ast->token_list, data);
		else if (astnodetype == NODE_TERMINAL)
			status = execute_command(ast->token_list, data);
		break ;
	}
	return (status);
}
