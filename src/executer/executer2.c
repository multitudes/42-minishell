/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 15:56:22 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

/*
removed these debug line for now
if (WIFEXITED(status))
	debug("child exited with status %d\n", WEXITSTATUS(status));
else
	debug("child did not exit normally\n");
*/
int	execute_command(t_list *tokenlist, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**argv;

	argv = get_args_from_tokenlist(tokenlist);
	if (!argv)
		return (error_set_status("malloc argv", 1));
	if (resolve_command_path(argv, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, (char **)data->env_arr->contents);
		exit_err_failure("minishell: execve failed\n");
	}
	else if (pid == -1)
		return (error_set_status("minishell: fork failed\n", 1));
	else
		waitpid(pid, &status, 0); 
	data->exit_status = WEXITSTATUS(status);
	return (status);
}

/*
we will not use WIFEXITED but maybe this ? (((*(int *)&(status)) & 0177) == 0)
or are we allowed to use it?
*/
int	get_status_of_children(pid_t pid1, pid_t pid2)
{
	int	status;

	status = -1;
	if (waitpid(pid1, &status, 0) == -1)
		status = error_set_status("waitpid 1", 1);
	status = WEXITSTATUS(status);
	if (waitpid(pid2, &status, 0) == -1)
		status = error_set_status("waitpid 2", 1);
	status = WEXITSTATUS(status);
	return (status);
}

int	execute_list(t_ast_node *ast, t_data *data)
{
	int			status;
	t_tokentype	tokentype;

	debug("NODE_LIST || &&");
	status = execute_ast(ast->left, data);
	tokentype = ((t_token *)ast->token_list->content)->type;
	if (status == 0 && tokentype == AND_IF)
	{
		debug("ANDTOKEN");
		status = execute_ast(ast->right, data);
	}
	else if (status != 0 && tokentype == OR_IF)
	{
		debug("ORTOKEN");
		status = execute_ast(ast->right, data);
	}
	debug("status now %d", status);
	return (status);
}

int	handle_first_child_process(t_data *data, t_ast_node *ast)
{
	if (close(data->pipe_fd[0]) == -1)
		return (exit_err_failure("close 1 error"));
	if (data->pipe_fd[1] != STDOUT_FILENO)
	{
		if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			return (exit_err_failure("dup2 1 error"));
		if (close(data->pipe_fd[1]) == -1)
			return (exit_err_failure("close 2 error"));
	}
	exit(execute_ast(ast->left, data));
}

int	handle_second_child_process(t_data *data, t_ast_node *ast)
{
	if (close(data->pipe_fd[1]) == -1)
		return (exit_err_failure("close 3 - child write end of the pipe"));
	if (data->pipe_fd[0] != STDIN_FILENO)
	{
		if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
			return (exit_err_failure("dup2 2 failed"));
		if (close(data->pipe_fd[0]) == -1)
			return (exit_err_failure("close fd 4"));
	}
	exit(execute_ast(ast->right, data));
}
