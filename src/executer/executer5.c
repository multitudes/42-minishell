/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 14:32:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 15:16:36 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "splash.h"
#include "splash_error.h"

/*
Checks if tokenlist contains a redirection token.
*/
bool	contains_redirection(t_list *tokenlist)
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
		return (perror_and_status("pipe", 1));
	pid1 = fork();
	if (pid1 == -1)
		return (perror_and_status("fork", 1));
	else if (pid1 == 0)
		handle_first_child_process(data, ast);
	pid2 = fork();
	if (pid2 == -1)
		return (perror_and_status("fork", 1));
	else if (pid2 == 0)
		handle_second_child_process(data, ast);
	if (close(data->pipe_fd[0]) == -1)
		return (perror_and_status("close pipe read", 1));
	if (close(data->pipe_fd[1]) == -1)
		return (perror_and_status("close pipe write", 1));
	return (get_status_of_children(pid1, pid2));
}

/*
 * Update the _ variable with the last command executed
 * in case of a single token we need to handle it as command
 * an add the path as well if not a builtin
 */
void	update_dollar_undersc_single_cmd(t_darray *env_arr, t_list *tokenlist)
{
	char	*cmd_lexeme;
	char	*cmd;

	cmd = NULL;
	cmd_lexeme = get_token_lexeme(tokenlist);
	if (ft_strncmp(cmd_lexeme, "env", 3) == 0)
		cmd = ft_strdup(cmd_lexeme);
	else 
	{
		cmd = create_path(cmd_lexeme, mini_get_env(env_arr, "PATH"));
		if (cmd == NULL)
			cmd = ft_strdup(cmd_lexeme);
	}
	if (update_env(env_arr, "_", cmd) == FALSE) 
		perror_and_null("update_env for '_'");
	free(cmd);
}

/*
 * Update the _ variable with the last command executed
 * in case of multiple token we need to put the last arg
 */
void	update_dollar_underscore(t_darray *env_arr, t_list *tokenlist)
{
	t_list	*last;

	last = ft_lstlast(tokenlist);
	if (count_tokens(tokenlist) == 1)
		update_dollar_undersc_single_cmd(env_arr, tokenlist);
	else if (update_env(env_arr, "_", get_token_lexeme(last)) == FALSE)
		perror_and_null("update_env for '_'");
}
