/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:48:30 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 16:41:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include <limits.h>
#include "debug.h"
#include "parser.h"
#include "splash.h"
#include "splash_error.h"
#include "libft.h"

/*
 * getting the status properly involves using WIFEXITED and WEXITSTATUS.
 * This is a utility function to return the status of the child.
 * WIFEXITED returns true if the child terminated normally and the status
 * of the child is returned by WEXITSTATUS. If the child did not terminate
 * normally, WIFSIGNALED will return true and WTERMSIG will return the signal
 * number that caused the child to terminate, so the status will be the 
 * signal number plus 128.
 * WIFSTOPPED returns true if the child process is stopped and WSTOPSIG will
 * return the signal number that caused the child to stop. The status will
 * be the signal number that caused the child to stop plus 128.
 * WIFCONTINUED is a status that is returned when the child is resumed by
 * a SIGCONT signal. The status will be 0.
 */
uint8_t	get_wait_status(int status)
{
	debug("child exited with status %d", status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (perror_and_status("child exit", 1));
}


/*
 * This function will execute the command in the tokenlist.
 * It will fork a child process and execute the command in the child.
 * The parent will wait for the child to finish and return the status.
 * The status will be the exit status of the child.
 * If the command is not found, the status will be 127.
 * If the command is found but not executable, the status will be 126.
 * If the command is found and executable, the status will be the exit status
 * of the child.
 */
int	execute_command(t_list *tokenlist, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**argv;

	status = 0;
	update_dollar_underscore(data->env_arr, data->ast->tokenlist);
	pid = fork();
	if (pid == 0)
	{
		argv = get_argv_from_tokenlist(&tokenlist);
		status = resolve_command_path(argv, mini_get_env(data->env_arr, "PATH"));
		if (status != 0)
			exit (status);
		// if (ft_strncmp(argv[0], "/usr/bin/awk", 13) == 0)
		// {	
		// 	// awk '{print $1}' filename.txt
		// 	// char *argv_awk[] = {"/usr/bin/awk", "-v", "RS= ", "{print}", NULL};
		// 	char *argv_awk[] = {"/usr/bin/awk", "{print $1}", "LICENSE", NULL};
		// 	argv = argv_awk;
		// }
		execve(argv[0], argv, (char **)data->env_arr->contents);
		exit(perror_and_status(argv[0], 126));
	}
	else if (pid == -1)
		return (perror_and_status("fork", EXIT_FAILURE));
	else
	{
		waitpid(pid, &status, 0);
		return (get_wait_status(status));
	}
}

/*
 * Called in the pipe case to get the status of the children.
*/
int	get_status_of_children(pid_t pid1, pid_t pid2)
{
	int status;
	int	finalstatus;
	
	finalstatus = -1;
	if (waitpid(pid1, &status, 0) == -1)
		finalstatus = perror_and_status("waitpid", 1);
	finalstatus = get_wait_status(status);
	if (waitpid(pid2, &status, 0) == -1)
		finalstatus = perror_and_status("waitpid", 1);
	finalstatus = get_wait_status(status);
	return (finalstatus);
}

/*
 * This function will execute the list.
 * There are two cses for a list node: AND_IF and OR_IF.
 */
int	execute_list(t_ast_node *ast, t_data *data)
{
	uint8_t	status;
	t_tokentype	tokentype;

	status = execute_ast(ast->left, data);
	tokentype = ((t_token *)ast->tokenlist->content)->type;
	if (status == 0 && tokentype == AND_IF)
		status = execute_ast(ast->right, data);
	else if (status != 0 && tokentype == OR_IF)
		status = execute_ast(ast->right, data);
	return (status);
}

/*
 * This function will close and clean up after the first child process.
 */
int	handle_first_child_process(t_data *data, t_ast_node *ast)
{
	if (close(data->pipe_fd[0]) == -1)
		perror_and_exit_with_status("child close read end of pipe", 1);
	if (data->pipe_fd[1] != STDOUT_FILENO)
	{
		if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			perror_and_exit_with_status("child dup2 write end of pipe", 1);
		if (close(data->pipe_fd[1]) == -1)
			perror_and_exit_with_status("child close write end of pipe", 1);
	}
	exit(execute_ast(ast->left, data));
}

/*
 * This function will close and clean up after the second child process.
 */
int	handle_second_child_process(t_data *data, t_ast_node *ast)
{
	if (close(data->pipe_fd[1]) == -1)
		perror_and_exit_with_status("child close write end of pipe", 1);
	if (data->pipe_fd[0] != STDIN_FILENO)
	{
		if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
			perror_and_exit_with_status("child dup2 read end of pipe", 1);
		if (close(data->pipe_fd[0]) == -1)
			perror_and_exit_with_status("child close read end of pipe", 1);
	}
	exit(execute_ast(ast->right, data));
}
