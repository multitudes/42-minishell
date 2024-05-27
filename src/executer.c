/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/27 16:49:03 by lbrusa           ###   ########.fr       */
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
				return (_return_err_failure("minishell: command not on path\n");
		argv[0] = cmd;
	}
	else
	{
		if (access(argv[0], X_OK) == -1)
			return (_return_err_failure("minishell: command not found\n"));
	}
	return 0;
}

/*
needs to be completely refactored
*/
int execute_command(t_list *tokenlist, t_data *data)
{
	pid_t pid;
	char *cmd;
	int status;
	char **argv;

	argv = get_args_from_tokenlist(tokenlist);
	if (!argv)
		return (_return_err_failure("malloc argv");
	if (!resolve_command_path(argv[0], data))
		return (_return_err_failure("minishell: command not found\n");
	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, (char **)data->env_arr->contents);
		write(2, "minishell: command not found\n", 30);
		exit(EXIT_FAILURE); // execvp returns only if there's an error
	}
	else if (pid == -1)
	{
		perror("fork");
	}
	else
	{
		waitpid(pid, &status, 0); 
	}
	data->exit_status = WEXITSTATUS(status);
	if (WIFEXITED(status))
		debug("child exited with status %d\n", WEXITSTATUS(status));
	else
		debug("child did not exit normally\n");
	return (status);
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
	t_token *token;
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
	token = (t_token *)tokenlist->content;
	debug("token type: %d", (t_tokentype)(token->type));
	// debug("lexeme %s\n", (char *)(token->lexeme));

	// true and false keywords
	if (astnodetype == NODE_TRUE)
	{
		debug("NODE_TRUE");

		return (0);
	}
	else if (astnodetype == NODE_FALSE)
	{
		debug("NODE_FALSE");
		return (1);
	}
	else if (astnodetype == NODE_LIST)
	{
		debug("NODE_LIST || &&");
		// get the token from the tokenlist
		t_token *token = (t_token *)ast->token_list->content;
		debug("node value, %s - status now %d", token->lexeme, status);
		status = execute_ast(ast->left, data);
		if (status == 0 && token->type == AND_IF)
		{
			debug("ANDTOKEN");
			status = execute_ast(ast->right, data);
		}
		else if (status != 0 && token->type == OR_IF)
		{
			debug("ORTOKEN");
			status = execute_ast(ast->right, data);
		}
		debug("status now %d", status);
		return (status);
	}
	else if (astnodetype == NODE_PIPELINE)
	{
		debug("NODE_PIPELINE");

		pid_t pid1, pid2;

		// create a pipe
		if (pipe(data->pipe_fd) == -1)
		{
			perror("pipe");
			return (0); // should be error
		}

		// fork the process
		pid1 = fork();
		debug("forked! pid1: %d", pid1);
		if (pid1 == -1)
		{
			perror("fork");
			return (0);
		}
		else if (pid1 == 0)
		{
			debug("first child process pid1: %d", pid1);
			// child process
			// redirect the stdout to the write end of the pipe
			// close the read end of the pipe (unused)
			if (close(data->pipe_fd[0]) == -1)
			{
				perror("close 1 - child read end of the pipe");
				return (0);
			}

			/*
			defensive check
			need duplicate and close one of the file descriptors
			*/
			if (data->pipe_fd[1] != STDOUT_FILENO)
			{
				if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 1");
					return (0);
				}
				if (close(data->pipe_fd[1]) == -1)
				{
					perror("close 2");
					return (0);
				}
			}

			// execute the left node
			status = execute_ast(ast->left, data);

			// debug("should not get here?");
			exit(status);
		}
		else
		{
			// parent falls through to create the next child
			debug("parent process pid1: %d falls through", pid1);
		}

		// fork the process again
		pid2 = fork();
		debug("forked second time! pid1: %d", pid2);

		if (pid2 == -1)
		{
			perror("fork");
			data->exit_status = 1;
			return (1);
		}
		else if (pid2 == 0)
		{
			// second child process
			debug("second child process pid1: %d", pid2);
			// close the write end of the pipe (unused)
			if (close(data->pipe_fd[1]) == -1)
			{
				perror("close 3 - child write end of the pipe");
				return (0);
			}

			/*
			defensive check
			need duplicate and close one of the file descriptors
			*/
			if (data->pipe_fd[0] != STDIN_FILENO)
			{
				if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
				{
					perror("dup2 2");
					return (0);
				}
				if (close(data->pipe_fd[0]) == -1)
				{
					perror("close 4");
					return (0);
				}
			}

			// execute the left node
			status = execute_ast(ast->right, data);

			exit(status);
		}
		else
		{
			// parent falls through to create the next child
			debug("parent process pid1: %d falls through", pid1);
		}

		/* Parent closes unused file descriptors for pipe, and waits for children */
		if (close(data->pipe_fd[0]) == -1)
		{
			perror("close 5");
			return (1);
		}
		if (close(data->pipe_fd[1]) == -1)
		{
			perror("close 6");

			return (1);
		}
		// with waitpid we can wait for a specific child process and get status
		int status;
		status = -1;
		if (waitpid(pid1, &status, 0) == -1)
		{
			perror("waitpid");
			data->exit_status = 1;
			return (1);
		}
		// we will not use WIFEXITED but maybe this ? (((*(int *)&(status)) & 0177) == 0)
		data->exit_status = WEXITSTATUS(status);
		debug("child exited with status %d\n", WEXITSTATUS(status));
		if (WIFEXITED(status))
		{
			debug("child exited with status %d\n", WEXITSTATUS(status));
		}
		else
		{
			debug("child did not exit normally\n");
		}
		status = -1;
		// wait for the second child
		if (waitpid(pid2, &status, 0) == -1)
		{
			perror("waitpid");
			data->exit_status = 1;
			return (1);
		}
		data->exit_status = WEXITSTATUS(status);
		debug("child exited with status %d\n", WEXITSTATUS(status));
		if (WIFEXITED(status))
		{
			debug("child exited with status %d\n", WEXITSTATUS(status));
		}
		else
		{
			debug("child did not exit normally\n");
		}
		return (status);
	}

	// to be continued with the other cases
	else if (astnodetype == NODE_BUILTIN)
	{
		execute_builtin(tokenlist, data);
		return (0);
	}
	else if (astnodetype == NODE_COMMAND)
	{
		debug("NODE_COMMAND");
		status = execute_command(tokenlist, data);
		return (status);
	}
	else if (astnodetype == NODE_TERMINAL)
	{
		debug("NODE_TERMINAL\n");
		status = execute_command(tokenlist, data);
		return (0);
	}
	else
		debug("not TERMINAL NODE\n");
	return (status);
}