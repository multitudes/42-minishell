/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/07 13:38:14 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include <limits.h>
/*
getting the status properly involves using WIFEXITED and WEXITSTATUS
so I this is an utility function to return the status of the child.
WIFEXITED returns true if the child terminated normally and the status
of the child is returned by WEXITSTATUS. If the child did not terminate
normally, WIFSIGNALED will return true and WTERMSIG will return the signal
number that caused the child to terminate, so the status will be the 
signal number plus 128.
WIFSTOPPED returns true if the child process is stopped and WSTOPSIG will
return the signal number that caused the child to stop. The status will
be the signal number that caused the child to stop plus 128.
WIFCONTINUED is a status that is returned when the child is resumed by
a SIGCONT signal. The status will be 0.
*/
int	get_wait_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	else
		return (status_and_perror("child did not exit normally\n", 1));
}


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

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		argv = get_argv_from_tokenlist(tokenlist);
		if (!resolve_command_path(argv, mini_get_env(data->env_arr, "PATH")))
			exit (127);
		debug("command and agrs: %s %s", argv[0], argv[1]);
		execve(argv[0], argv, (char **)data->env_arr->contents);
		exit_and_print_err(NULL, 127);
	}
	else if (pid == -1)
		return (status_and_perror("minishell: fork failed\n", EXIT_FAILURE));
	else
		waitpid(pid, &status, 0);
	return (get_wait_status(status));
}

/*
we will not use WIFEXITED but maybe this ? (((*(int *)&(status)) & 0177) == 0)
or are we allowed to use it?
*/
int	get_status_of_children(pid_t pid1, pid_t pid2)
{
	int status;
	int	finalstatus;
	
	finalstatus = -1;
	if (waitpid(pid1, &status, 0) == -1)
		finalstatus = status_and_perror("waitpid 1", 1);
	finalstatus = get_wait_status(status);
	if (waitpid(pid2, &status, 0) == -1)
		finalstatus = status_and_perror("waitpid 2", 1);
	finalstatus = get_wait_status(status);
	debug("status of my children %d", finalstatus);
	return (finalstatus);
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
		exit_and_print_err("close 1 error", 1);
	if (data->pipe_fd[1] != STDOUT_FILENO)
	{
		if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			exit_and_print_err("dup2 1 error", 1);
		if (close(data->pipe_fd[1]) == -1)
			exit_and_print_err("close 2 error", 1);
	}
	exit(execute_ast(ast->left, data));
}

int	handle_second_child_process(t_data *data, t_ast_node *ast)
{
	if (close(data->pipe_fd[1]) == -1)
		exit_and_print_err("close 3 - child write end of the pipe", 1);
	if (data->pipe_fd[0] != STDIN_FILENO)
	{
		if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
			exit_and_print_err("dup2 2 failed", 1);
		if (close(data->pipe_fd[0]) == -1)
			exit_and_print_err("close fd 4", 1);
	}
	exit(execute_ast(ast->right, data));
}
