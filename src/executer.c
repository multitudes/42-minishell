/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/27 18:16:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "built_ins.h"

/*
posix compliant use of the environ variable but wecan discuss this
*/
extern char **environ;

/*
when I need to free a string array like the envpaths
*/
int free_array(char **envpaths)
{
	int i = 0;
	while (envpaths[i])
	{
		free(envpaths[i]);
		i++;
	}
	free(envpaths);
	return (0);
}

/*
In our data struct we have the environment variables in a dynamic array
mini_get_env will get the path variable and return it
as a string array.
base is the command we are looking for.
*/
char *create_path(char *base, t_data *data)
{
	int i;
	char *commandpath;
	char **envpaths;

	i = 0;
	envpaths = ft_split(mini_get_env(data, "PATH"), ':');
	while (envpaths[i])
	{
		commandpath = ft_strjoin3(envpaths[i], "/", base);
		if (access(commandpath, X_OK) == 0)
		{
			free_array(envpaths);
			return (commandpath);
		}
		else
			free(commandpath);
		i++;
	}
	free_array(envpaths);
	return (NULL);
}
 
int count_tokens(t_list *tokenlist) 
{
	int count;

	count = 0;
	while (tokenlist) {
		count++;
		tokenlist = tokenlist->next;
	}
	return (count);
}


char **get_args_from_tokenlist(t_list *tokenlist)
{
	int 	i;
	int		count;
	char 	**args;
	
	i = 0;
	count = count_tokens(tokenlist);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		perror("malloc args");
		return (NULL);
	}
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
			return (_error_with_status("minishell: command not on path\n", data));
		argv[0] = cmd;
	}
	else
	{
		if (access(argv[0], X_OK) == -1)
			return (_error_with_status("minishell: command not found\n", data));
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
		return (_error_with_status("malloc argv", data));
	if (resolve_command_path(argv, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, (char **)data->env_arr->contents);
		_exit_err_failure("minishell: execve failed\n");	
	}
	else if (pid == -1)
		return (_error_with_status("minishell: fork failed\n", data));
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
		return (_error_with_status("waitpid 1", data));	
	data->exit_status = WEXITSTATUS(status);
	status = -1;
	if (waitpid(pid2, &status, 0) == -1)
		return (_error_with_status("waitpid 2", data));
	data->exit_status = WEXITSTATUS(status);
	return (status);
}

int execute_list(t_ast_node *ast, t_data *data)
{
	int status;

	status = 0;
	debug("NODE_LIST || &&");
	// get the token from the tokenlist
	t_tokentype tokentype = ((t_token *)ast->token_list->content)->type;
	status = execute_ast(ast->left, data);
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


int	execute_pipeline(t_ast_node *ast, t_data *data)
{
	debug("NODE_PIPELINE");
	pid_t pid1;
	pid_t pid2;

	// create a pipe
	if (pipe(data->pipe_fd) == -1)
		return (_error_with_status("pipe error", data));

	// fork the process
	pid1 = fork();
	debug("forked! pid1: %d", pid1);
	if (pid1 == -1)
		return (_error_with_status("fork 1 error", data));
	else if (pid1 == 0)
	{
		debug("first child process pid1: %d", pid1);
		// child process
		// redirect the stdout to the write end of the pipe
		// close the read end of the pipe (unused)
		if (close(data->pipe_fd[0]) == -1)
			return (_error_with_status("close 1 error", data));
		/*
		defensive check
		need duplicate and close one of the file descriptors
		*/
		if (data->pipe_fd[1] != STDOUT_FILENO)
		{
			if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
				return (_error_with_status("dup2 1 error", data));
			if (close(data->pipe_fd[1]) == -1)
				return (_error_with_status("close 2 error", data));
		}
		// execute the left node and exit child process
		exit(execute_ast(ast->left, data));
	}
	else
		debug("parent process pid1: %d falls through", pid1);
	pid2 = fork();
	if (pid2 == -1)
		return (_error_with_status("fork 2 failed", data));
	else if (pid2 == 0)
	{
		// second child process
		debug("second child process pid1: %d", pid2);
		// close the write end of the pipe (unused)
		if (close(data->pipe_fd[1]) == -1)
			return (_error_with_status("close 3 - child write end of the pipe", data));
		/*
		defensive check
		need duplicate and close one of the file descriptors
		*/
		if (data->pipe_fd[0] != STDIN_FILENO)
		{
			if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
				return (_error_with_status("dup2 2 failed", data));
			if (close(data->pipe_fd[0]) == -1)
				return (_error_with_status("close fd 4", data));
		}
		// execute the left node and exit child process
		exit(execute_ast(ast->right, data));
	}
	else
	{
		// parent process
		debug("parent process pid2: %d", pid2);
	}
	/* Parent closes unused file descriptors for pipe, and waits for children */
	if (close(data->pipe_fd[0]) == -1)
		return (_error_with_status("close fd 5", data));
	if (close(data->pipe_fd[1]) == -1)
		return (_error_with_status("close fd 6", data));
	return (get_status_of_children(pid1, pid2, data));
}

/*
also needs to be refactored : TODO

traverse the ast and execute the commands node by node left to right
pipes
To handle the piping in your code, you need to create a pipe using the
pipe() function, then fork the process. In the child process,
you need to redirect the stdout to the write end of the pipe,
then execute the left node. In the parent process,
you need to wait for the child to finish, then redirect the stdin
to the read end of the pipe, and execute the right node.
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
	else
		debug("not TERMINAL NODE\n");
	return (status);
}