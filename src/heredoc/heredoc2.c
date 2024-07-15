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

static void remove_backslash(char **str)
{
    char    *temp;

    debug("remove backslash");
    if (*str && (*str)[0] != '\\')
        return ;
    else if (*str && (*str)[0] == '\\' \
            && ((*str)[1] == '\0' || (*str)[1] == '$'))
    {
        temp = ft_strdup((*str) + 1);
        free (*str);
        *str = temp;
    }
}

/*
Frees the memory used for storing delimiter lexemes.
*/
void    free_heredoc(t_heredoc *heredoc)
{
    int i;

    debug("free heredoc content (delimiter and filenames)");
    i = 0;
    while (i < heredoc->delim_count)
    {
        free(heredoc->delim[i]);
        free(heredoc->file[i++]);
        debug("Reached end of free heredoc content loop (i = %i)", i - 1);
    }
}

/*
Read and save content of final heredoc.
 // add check for ()syntax errorTODO
*/
static bool read_heredoc(t_heredoc *heredoc, t_data *data, int i)
{
    char    *line;
    char    *temp;
    int     temp_fd;

    debug("read heredoc");
    debug("Filename used for heredoc: %s", heredoc->file[i]);
    temp_fd = open(heredoc->file[i], O_WRONLY | O_CREAT | O_TRUNC, \
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (temp_fd < 0)
        return (perror_and_bool("setup heredoc", false));
    line = readline("> ");
    while (line && ft_strcmp(heredoc->delim[i], line))
    {
        if (line && heredoc->expansion[i] == true && ft_strchr(line, '$') \
                && line[0] != '\\')
        {
            temp = replace_dollar_vars(data, line);
            free(line);
            line = temp;
        }
        remove_backslash(&line);
        if (!ft_write(temp_fd, line) || !ft_write(temp_fd, "\n"))
        {
            free(line);
            return (false);
        }
        free(line);
        if (g_signal == SIGINT)
            return (false);
        debug("Current heredoc delimiter to match: %s", heredoc->delim[i]);
        line = readline("> ");
    }
	if (line == NULL)
		ft_write(1, \
            "minishell: warning: here-document delimited by end-of-file\n");
    free(line);
    close(temp_fd);
    return (true);
}

/*
Prompt for each delimiter and advancing to next heredoc/delimiter.
*/
static bool advance_to_final_delim(t_heredoc *heredoc, t_data *data)
{
    int     i;

    i = 0;
    debug("advance to final delim");
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
    debug("Process heredoc");
    if (!advance_to_final_delim(heredoc, data))
	{
        debug("False return to process heredoc");
		free_heredoc(heredoc);
        g_signal = 0;
		return (false);
	}
    debug("True return to process heredoc");
	free_heredoc(heredoc);
    debug("Return to process heredoc after freeing");
    return (true);
}
