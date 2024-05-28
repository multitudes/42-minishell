/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 11:50:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "minishell.h"

/*
posix compliant use of the environ variable but wecan discuss this
*/
extern char **environ;

/*
Since until now we store the token as linked list
we convert it to a char array for the execve function
*/ 
char **get_args_from_tokenlist(t_list *tokenlist)
{
	int 	i;
	int		count;
	char 	**args;
	
	i = 0;
	count = count_tokens(tokenlist);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	while (tokenlist)
	{
		t_token *token = (t_token *)tokenlist->content;
		args[i++] = token->lexeme;
		tokenlist = tokenlist->next;
	}
	args[i] = NULL;
	return (args);
}

/*
resolve_command_path will check if the command is in the PATH
or if it is an absolute path. 
if it cannot be resolved it will return 1
*/
int resolve_command_path(char **argv, t_data *data)
{
	char *cmd;
	
	cmd = NULL;
	if (ft_strchr(argv[0], '/') == NULL)
	{
		cmd = create_path(argv[0], data);
		if (!cmd)
			return (error_set_status("minishell: command not on path\n", data));
		argv[0] = cmd;
	}
	else
	{
		if (access(argv[0], X_OK) == -1)
			return (error_set_status("minishell: command not found\n", data));
	}
	return 0;
}

/*

removed these debug line for now
if (WIFEXITED(status))
	debug("child exited with status %d\n", WEXITSTATUS(status));
else
	debug("child did not exit normally\n");
*/
int execute_command(t_list *tokenlist, t_data *data)
{
	pid_t pid;
	int status;
	char **argv;

	argv = get_args_from_tokenlist(tokenlist);
	if (!argv)
		return (error_set_status("malloc argv", data));
	if (resolve_command_path(argv, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, (char **)data->env_arr->contents);
		exit_err_failure("minishell: execve failed\n");	
	}
	else if (pid == -1)
		return (error_set_status("minishell: fork failed\n", data));
	else
		waitpid(pid, &status, 0); 
	data->exit_status = WEXITSTATUS(status);
	return (status);
}

/*
we will not use WIFEXITED but maybe this ? (((*(int *)&(status)) & 0177) == 0)
or are we allowed to use it?
*/
int	get_status_of_children(pid_t pid1, pid_t pid2, t_data *data)
{		
	int status;

	status = -1;
	if (waitpid(pid1, &status, 0) == -1)
		return (error_set_status("waitpid 1", data));	
	data->exit_status = WEXITSTATUS(status);
	status = -1;
	if (waitpid(pid2, &status, 0) == -1)
		return (error_set_status("waitpid 2", data));
	data->exit_status = WEXITSTATUS(status);
	return (status);
}

int execute_list(t_ast_node *ast, t_data *data)
{
	int status;
	t_tokentype tokentype;

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

int handle_first_child_process(t_data *data, t_ast_node *ast) 
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


int handle_second_child_process(t_data *data, t_ast_node *ast) 
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


int	execute_pipeline(t_ast_node *ast, t_data *data)
{
	pid_t pid1;
	pid_t pid2;

	if (pipe(data->pipe_fd) == -1)
		return (error_set_status("pipe error", data));
	pid1 = fork();
	if (pid1 == -1)
		return (error_set_status("fork 1 error", data));
	else if (pid1 == 0)
	 	handle_first_child_process(data, ast);
	pid2 = fork();
	if (pid2 == -1)
		return (error_set_status("fork 2 failed", data));
	else if (pid2 == 0)
		handle_second_child_process(data, ast);
	if (close(data->pipe_fd[0]) == -1)
		return (error_set_status("close fd 5", data));
	if (close(data->pipe_fd[1]) == -1)
		return (error_set_status("close fd 6", data));
	return (get_status_of_children(pid1, pid2, data));
}

/*
Traverse the ast and execute the commands node by node 
left to right
*/
int execute_ast(t_ast_node *ast, t_data *data)
{
	int status;
	t_list *tokenlist;
	t_nodetype astnodetype;

	status = 0;
	if (ast == NULL)
		return (0);
	debug("\nexecute:");
	debug("ast node type: %d ", ast->type);
	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
	astnodetype = ast->type;
	if (astnodetype == NODE_LIST)
		status = execute_list(ast, data);
	else if (astnodetype == NODE_PIPELINE)
		status = execute_pipeline(ast, data);
	else if (astnodetype == NODE_BUILTIN)
		execute_builtin(tokenlist, data);
	else if (astnodetype == NODE_COMMAND)
		status = execute_command(tokenlist, data);
	else if (astnodetype == NODE_TERMINAL)
		status = execute_command(tokenlist, data);
	return (status);
}
