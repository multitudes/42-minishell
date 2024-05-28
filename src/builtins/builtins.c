/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 22:01:13 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/28 08:26:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "scanner.h"
#include "environment.h"
#include "history.h"

/*
NOT yet implemented - TODO
true and false are shell builtins that do nothing except return an exit status of 0 and 1, respectively.
we need them eventually for the bonus... true && false || true etc
*/
int    execute_builtin(t_list *tokenlist, t_data *data)
{
	t_token *token;
	char *lexeme;
	int		status;
	(void)data; //I think this does not do anything!?

	token = (t_token *)tokenlist->content;
	lexeme = (char *)token->lexeme;
	status = 0;
	if (ft_strncmp(lexeme, "echo", 5) == 0)
	{
		debug("echo builtin---- \n");
		while (tokenlist)
		{
			token = (t_token *)tokenlist->content;
			debug("lexeme: %s\n", token->lexeme);
			tokenlist = tokenlist->next;
		}
	}
	else if (ft_strncmp(lexeme, "cd", 3) == 0)
	{
		debug("cd builtin\n");
	}
	else if (ft_strncmp(lexeme, "pwd", 4) == 0)
	{
		debug("pwd builtin\n");
	}
	else if (ft_strncmp(lexeme, "export", 7) == 0)
	{
		debug("export builtin\n");
	}
	else if (ft_strncmp(lexeme, "unset", 6) == 0)
	{
		debug("unset builtin\n");
	}
	else if (ft_strncmp(lexeme, "env", 4) == 0)
		status = execute_env_builtin(data);
	else if (ft_strncmp(lexeme, "exit", 5) == 0)
	{
		debug("exit builtin\n");
	}
	else if (ft_strncmp(lexeme, "true", 5) == 0)
	{
		debug("true builtin\n");
	}
	else if (ft_strncmp(lexeme, "false", 6) == 0)
	{
		debug("false builtin\n");
	}
	else if (ft_strncmp(data->input, "history -c", 11) == 0 || ft_strncmp(data->input, "history --clear", 16) == 0)
	{
		// debug("clearing history\n");
		clear_hist_file();
		rl_clear_history();
	}
	else if (ft_strncmp(data->input, "history", 7) == 0)
	{
		print_history();
	}
	else
	{
		debug("not an implemented builtin");
	}
	return (status);
}

int	execute_env_builtin(t_data *data)
{
	int	status;

	debug("env builtin");
	status = 0;
	status = print_env(data->env_arr);
	return (status);
}