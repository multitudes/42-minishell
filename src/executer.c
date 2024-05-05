/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 16:55:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"

extern char **environ;

void	execute_builtin(t_list *tokenlist, t_mini_data *data)
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
	else
	{
		debug("not an implemented builtin\n");
	}
	
}


/*
split the PATH variable into base paths
check if the base path is in the PATH variable
if it is return true
else return false
*/
bool is_on_path(char *base, t_mini_data *data)
{
	char **envpaths = ft_split(mini_get_env(data, "PATH"), ':');

	int i = 0;
	while (envpaths[i])
	{
		debug("path: %s", envpaths[i]);
		if (ft_strncmp(envpaths[i], base, ft_strlen(base)) == 0)
		{
			return (true);
		}
		i++;
	}
	return (0);
}


int	execute_command(t_list *tokenlist, t_mini_data *data)
{
	t_token *token;
	(void)data;
	char *argv[100];
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
	// this is starting with / so it is a absolute path
	if (argv[0][0] == '/')
	{
		// check if the path is in the PATH variable
		char *base = ft_strdup(argv[0]);
		char* slash = strrchr(base, '/');
		if (slash != NULL && slash != base) 
		{
			*slash = '\0';
		}
		debug("base: %s", base);
		// look for the base path in the env variable PATH
		if (!is_on_path(base, data))
		{
			debug("not valid path\n");
		}
		else 
		{
			debug("path found\n");
			if (access(argv[0], X_OK) == -1)
			{
				perror("access");
			}
		}
	}
	else 
	{
		// print getcwd
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			debug("Current working dir: %s", cwd);
			// check if the current dir plus the command is executable
			// add the current directory and the argv[0] 
			char *new_command = ft_strjoin3(cwd, "/", argv[0]);
			debug("new_command: %s", new_command);
			if (access(new_command, X_OK) == 0)
			{
				debug("new_command found");
			}
			else
			{
				perror("access");
			}
		}
		else
		{
			perror("getcwd");
		}
		// check the PATH variable for a path corresponding to the command
		
	}

    execve(argv[0], argv, (char **)data->env_arr->contents);

    // If execve returns at all, an error occurred.
	write(2, "minishell: command not found\n", 30);
    perror("execve");
    return 1;
}
/*
traverse the ast and execute the commands node by node left to right
pipes
To handle the piping in your code, you need to create a pipe using the 
pipe() function, then fork the process. In the child process, 
you need to redirect the stdout to the write end of the pipe, 
then execute the left node. In the parent process, 
you need to wait for the child to finish, then redirect the stdin 
to the read end of the pipe, and execute the right node.
*/
int	execute_ast(t_ast_node *ast, t_mini_data *data)
{
	t_list *tokenlist;
	t_token *token;
	t_nodetype astnodetype;

	if (ast == NULL)
		return (0);
	debug("\nexecute:");
	debug("ast node type: %d ", ast->type);
	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
	astnodetype = ast->type;
	token = (t_token *)tokenlist->content;
	// check if the token list is a builtin
	debug("token type: %d", (t_tokentype)(token->type));
	// debug("lexeme %s\n", (char *)(token->lexeme));

	if (astnodetype == NODE_PIPE)
	{
		debug("NODE_PIPE");

		
		// create a pipe
		int pfd[2];
		pid_t pid1, pid2;
		
		if (pipe(pfd) == -1)
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
			if (close(pfd[0]) == -1)
			{
				perror("close 1 - child read end of the pipe");
				return (0);
			}
			
			/* 
			defensive check
			need duplicate and close one of the file descriptors	
			*/
			if (pfd[1] != STDOUT_FILENO) 
			{
				if (dup2(pfd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 1");
					return (0);
				}
        		if (close(pfd[1]) == -1)
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
			if (close(pfd[1]) == -1)
			{
				perror("close 3 - child write end of the pipe");
				return (0);
			}
			
			/* 
			defensive check
			need duplicate and close one of the file descriptors	
			*/
			if (pfd[0] != STDIN_FILENO) 
			{
				if (dup2(pfd[0], STDIN_FILENO) == -1)
				{
					perror("dup2 2");
					return (0);
				}
        		if (close(pfd[0]) == -1)
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
		if (close(pfd[0]) == -1)
		{
			perror("close 5");
			return (0);
		}
		if (close(pfd[1]) == -1)
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
	else
		debug("not TERMINAL NODE\n");
	return (0);
}