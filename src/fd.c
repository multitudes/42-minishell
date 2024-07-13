/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 12:48:13 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 16:34:24 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "fd.h"
#include "minishell.h"
#include "unistd.h"

void	save_fds(t_data *data)
{
	debug("Save for later restoration STDIN: %i, STDOUT: %i, STDERR: %i", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stderr = dup(STDERR_FILENO);
	if (data->original_stdout == -1 || data->original_stdin == -1 || data->original_stderr == -1)
	{
		data->exit_status = 1;
		perror("minishell: dup");
	}
}

/*
 * Restores the original file descriptors
 * the only time we needd to restore is when executing a single builtin command
 * because in the other cases we always fork and exec
 * yes I think we need to close the original fds
 * https://stackoverflow.com/a/11042581/9497800
*/

// void	retore_stdin()
// {
// 	data
// }

void	restore_fds(t_data *data)
{
	if (data->original_stdin != -1)
	{
		// if (data->original_stdin != STDIN_FILENO)
		// {
			if (dup2(data->original_stdin, STDIN_FILENO) == -1)
			{
				data->exit_status = 1;
				perror("minishell: dup2");
			}
		// }
		if (close(data->original_stdin) == -1)
		{
			data->exit_status = 1;
			perror("minishell: close");
		}
	}
	if (data->original_stdout != -1)
	{
		// if (data->original_stdout != STDOUT_FILENO)
		// {
			if (dup2(data->original_stdout, STDOUT_FILENO) == -1)
			{
				data->exit_status = 1;
				perror("minishell: dup2");
			}
		// }
		if (close(data->original_stdout) == -1)
		{
			data->exit_status = 1;
			perror("minishell: close");
		}
	}
		if (data->original_stderr != -1)
	{
		// if (data->original_stderr != STDERR_FILENO)
		// {
			if (dup2(data->original_stderr, STDERR_FILENO) == -1)
			{
				data->exit_status = 1;
				perror("minishell: dup2");
			}
		// }
		if (close(data->original_stderr) == -1)
		{
			data->exit_status = 1;
			perror("minishell: close");
		}
	}
	data->original_stdout = -1;
	data->original_stdin = -1;
	data->original_stderr = -1;
}

void	init_fds(t_data *data)
{
	data->original_stdin = -1;
	data->original_stdout = -1;
	data->original_stderr = -1;
}