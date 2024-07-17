/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 02:19:34 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 12:50:14 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "analyser.h"
#include "scanner.h"
#include "builtins.h"
#include "utils.h"
#include "splash_error.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

static bool	process_line(t_heredoc *heredoc, int i, int heredoc_fd, char *line)
{
	bool	status;

	status = true;
	if (line && ft_strcmp(heredoc->delim[i], line))
	{
		if (!ft_write(heredoc_fd, line) || !ft_write(heredoc_fd, "\n"))
			status = false;
		else
			status = true;
	}
	else if (line == NULL)
	{
		ft_write(1, \
		"minishell: warning: here-document delimited by end-of-file (wanted '");
		ft_write(1, heredoc->delim[i]);
		ft_write(1, "')\n");
		status = true;
	}
	free(line);
	return (status);
}

/*
Open a file for writing heredoc content to
and update the file descriptor passed as argument.
*/
static bool	open_heredoc_file(char *file, int *fd)
{
	*fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, \
							S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (*fd < 0)
		return (perror_and_bool("setup heredoc", false));
	return (true);
}

/*
Read and save content of final heredoc.
TODO add check for ()syntax errors
*/
static bool	read_heredoc(t_heredoc *heredoc, t_data *data, int i)
{
	char	*line;
	int		heredoc_fd;

	heredoc_fd = -1;
	if (!open_heredoc_file(heredoc->file[i], &heredoc_fd))
		return (false);
	line = readline("> ");
	while (line && ft_strcmp(heredoc->delim[i], line))
	{
		line = expand_heredoc(heredoc->expansion[i], data, line);
		if (!process_line(heredoc, i, heredoc_fd, line))
			return (false);
		if (g_signal == SIGINT)
			return (false);
		line = readline("> ");
	}
	process_line(heredoc, i, heredoc_fd, line);
	close(heredoc_fd);
	return (true);
}

/*
Prompt for each delimiter and advancing to next heredoc/delimiter.
*/
static bool	advance_to_final_delim(t_heredoc *heredoc, t_data *data)
{
	int	i;

	i = 0;
	while (i < heredoc->delim_count)
	{
		if (!read_heredoc(heredoc, data, i))
			return (false);
		i++;
	}
	return (true);
}

/*
Reads heredoc content. Only input for final delimiter
gets saved and passed to stdin of any commands.
*/
bool	process_heredoc(t_heredoc *heredoc, t_data *data)
{
	if (!advance_to_final_delim(heredoc, data))
	{
		free_heredoc(heredoc);
		g_signal = 0;
		return (false);
	}
	free_heredoc(heredoc);
	return (true);
}
