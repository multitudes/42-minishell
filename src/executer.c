/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 10:34:49 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

/*
posix compliant use of the environ variable but wecan discuss this
*/
extern char **environ;

/*
NOT yet implemented - TODO
true and false are shell builtins that do nothing except return an exit status of 0 and 1, respectively.
we need them eventually for the bonus... true && false || true etc
*/
void	execute_builtin(t_list *tokenlist, t_data *data)
{
	t_token *token;
	char *lexeme;
	(void)data;

	token = (t_token *)tokenlist->content;
	lexeme = (char *)token->lexeme;
	if (ft_strncmp(lexeme, "echo", 5) == 0)
	{
		debug("echo ---- \n");
		while (tokenlist)
		{
			token = (t_token *)tokenlist->content;
			debug("lexeme: %s\n", token->lexeme);
			tokenlist = tokenlist->next;
		}
	}
	else if (ft_strncmp(lexeme, "cd", 3) == 0)
	{
		debug("cd\n");
	}
	else if (ft_strncmp(lexeme, "pwd", 4) == 0)
	{
		debug("pwd\n");
	}
	else if (ft_strncmp(lexeme, "export", 7) == 0)
	{
		debug("export\n");
	}
	else if (ft_strncmp(lexeme, "unset", 6) == 0)
	{
		debug("unset\n");
	}
	else if (ft_strncmp(lexeme, "env", 4) == 0)
	{
		debug("env\n");
	}
	else if (ft_strncmp(lexeme, "exit", 5) == 0)
	{
		debug("exit\n");
	}
	else if (ft_strncmp(lexeme, "true", 5) == 0)
	{
		debug("true\n");
	}
	else if (ft_strncmp(lexeme, "false", 6) == 0)
	{
		debug("false\n");
	}
	else
	{
		debug("not an implemented builtin\n");
	}
	
}

/*
when I need to free a string array like the envpaths
*/
int	free_array(char **envpaths)
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
char	*create_path(char *base, t_data *data)
{
	int	i; 
	char *commandpath;
	char **envpaths;

	i = 0;
	envpaths = ft_split(mini_get_env(data, "PATH"), ':');
	while (envpaths[i])
	{
		// debug("path: %s", envpaths[i]);
		commandpath = ft_strjoin3(envpaths[i], "/", base);
		if (access(commandpath, X_OK) == 0)
		{
			free_array(envpaths);
			return (commandpath);
		}
		else
			// debug("command not found");
		free(commandpath);
		i++;
	}
	free_array(envpaths);
	return (NULL);
}

/*
needs to be completely refactored
*/
int	execute_command(t_list *tokenlist, t_data *data)
{
	t_token *token;
	(void)data;
	char *argv[100];
	char *cmd;
	int i;

	i = 0;
	// convert token list to argvs
	while (tokenlist)
	{
		token = (t_token *)tokenlist->content;
		argv[i] = token->lexeme;
		debug("argv[%d]: -%s-", i, argv[i]);
		i++;
		tokenlist = tokenlist->next;
	}
	argv[i] = NULL;

	// If the command name contains no slashes, the shell attempts to locate it. 
	if (ft_strchr(argv[0], '/') == NULL)
	{

		// check if the path is in the PATH variable
		cmd = create_path(argv[0], data);
		if (!cmd)
		{
			debug("not on path\n");
		}
		else 
		{
			debug("command found on path: %s", cmd);
			argv[0] = cmd;
		}
	}
	else 
	{
		// trying to execute the command with access
		if (access(argv[0], X_OK) == -1)
		{
			debug("command not executable\n");
			return 1;
		}
		else
			debug("command executable\n");
	}

    execve(argv[0], argv, (char **)data->env_arr->contents);

    // If execve returns at all, an error occurred.
	write(2, "minishell: command not found\n", 30);
    perror("execve");
    return 127;
}

/*
also needs to be refactored

traverse the ast and execute the commands node by node left to right
pipes
To handle the piping in your code, you need to create a pipe using the 
pipe() function, then fork the process. In the child process, 
you need to redirect the stdout to the write end of the pipe, 
then execute the left node. In the parent process, 
you need to wait for the child to finish, then redirect the stdin 
to the read end of the pipe, and execute the right node.
*/
int	execute_ast(t_ast_node *ast, t_data *data)
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
		else if ( pid1 == 0 )
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
			execute_ast(ast->left, data);
		
			// debug("should not get here?");
			exit(1);
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
			return (0);
		}
		else if ( pid2 == 0 )
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
			execute_ast(ast->right, data);
		
			exit(1);
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
			return (0);
		}
		if (close(data->pipe_fd[1]) == -1)
		{
			perror("close 6");
			return (0);
		}
		// with waitpid we can wait for a specific child process and get status
		int status;
		status = -1;
		if (waitpid(pid1, &status, 0) == -1)
		{
			perror("waitpid");
			return (0);
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
			return (0);
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
		return (1);	
	}

	// to be continued with the other cases
	else if (astnodetype == NODE_BUILTIN)
	{
		execute_builtin(tokenlist, data);
		return (0);
	}
	else if (astnodetype == NODE_COMMAND )
	{
		debug("NODE_COMMAND");
		execute_command(tokenlist, data);
		return (0);
	}
	else if (astnodetype == NODE_TERMINAL)
	{
		debug("NODE_TERMINAL\n");
		return (0);
	}
	else
		debug("not TERMINAL NODE\n");
	return (0);
}