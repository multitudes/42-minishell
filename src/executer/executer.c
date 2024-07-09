/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/09 15:30:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "minishell.h"
#include "splash_error.h"
#include "builtins.h"
#include "analyser.h"
#include "heredoc.h"
#include "darray.h"
#include "scanner.h"

/*
posix compliant use of the environ variable but wecan discuss this
*/
extern char	**environ;

/*
Checks if tokenlist contains a redirection token.
*/
static bool	contains_redirection(t_list *tokenlist)
{
	t_tokentype	tokentype;
	while (tokenlist)
	{
		tokentype = get_token_type(tokenlist);
		if (is_redirection_token(tokentype))
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

void	update_dollar_underscore(t_darray *env_arr, t_list *tokenlist)
{
	char	*cmd;
	t_list	*last;

	if (count_tokens(tokenlist) == 1)
	{
		cmd = create_path(get_token_lexeme(tokenlist), mini_get_env(env_arr, "PATH"));
		if (cmd == NULL || ft_strncmp(get_token_lexeme(tokenlist), "env", 3) == 0)
			cmd = ft_strdup(get_token_lexeme(tokenlist));
		if (update_env(env_arr, "_", cmd) == FALSE)
			perror("in update_env for _ ");
		debug("cmd: %s ===================================", cmd);
		free(cmd);
	}
	else 
	{
		last = ft_lstlast(tokenlist);
		debug("last token to be added in $_: %s", ((t_token *)last->content)->lexeme);
		if (update_env(env_arr, "_", ((t_token *)last->content)->lexeme) == FALSE)
			perror("in update_env for _ ");
	}	
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
	debug("\nexecute ast (node type: %d)", ast->type);;
	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
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
			// debug("contains redirection (check)");
			status = execute_redirection(&ast); // double pointer should not be needed here.
			debug("Status after execute redirection: %i", status);
			if (status == 0)
				continue ;
		}
		else if (is_heredoc(ast->token_list))
		{
			debug("is heredoc"); // TODO look for updating $_ somewhere
			status = execute_heredoc(ast, data);
		}
		else if (astnodetype == NODE_BUILTIN)
		{	
			update_dollar_underscore(data->env_arr, ast->token_list);
			status = execute_builtin(ast->token_list, data);
		}
		else if (astnodetype == NODE_COMMAND || astnodetype == NODE_TERMINAL)
		{
			update_dollar_underscore(data->env_arr, ast->token_list);
			status = execute_command(ast->token_list, data);
		}
		restore_fds(data);
		break ;
	}
	return (status);
}
