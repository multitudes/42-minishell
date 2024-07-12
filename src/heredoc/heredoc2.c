/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 02:19:34 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/06 18:03:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "analyser.h"
#include "scanner.h"
#include "builtins.h"
#include "utils.h"
#include "splash_error.h"
#include <libft.h>
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
    else if (*str && (*str)[0] == '\\' && ((*str)[1] == '\0' || (*str)[1] == '$'))
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
Increases buffer for heredoc if addition of next line would surpass buffer size.
*/
// static bool increase_heredoc_size(t_heredoc *heredoc, int len)
// {
//     char    *new_buffer;

//     heredoc->buffer_size = heredoc->buffer_size + len + HEREDOC_BUFFER;
//     new_buffer = ft_calloc(heredoc->buffer_size, sizeof(char));
//     if (!new_buffer)
//         return(false_and_print("minishell: error: heredoc memory allocation"));
//     ft_strlcpy(new_buffer, heredoc->buffer, heredoc->buffer_size);
//     free(heredoc->buffer);
//     heredoc->buffer = new_buffer;
//     return (true);
// }

/*
Read and save content of final heredoc.
*/
static bool read_heredoc(t_heredoc *heredoc, t_data *data, int i)
{
    char    *line;
    char    *temp;
    int     temp_fd;

    debug("read heredoc");
    debug("Filename used for heredoc: %s", heredoc->file[i]);
    temp_fd = open(heredoc->file[i], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (temp_fd < 0)
        return (false_and_perr("minishell: setup heredoc"));
    line = readline("> ");
    while (line && ft_strcmp(heredoc->delim[i], line))
    {
        if (line && heredoc->expansion[i] == true && ft_strchr(line, '$') && line[0] != '\\')
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
        // if (heredoc->heredoc_len + ft_strlen(line) + 1 >= heredoc->buffer_size)
        // {
        //     if (!increase_heredoc_size(heredoc, (int)ft_strlen(line)))
        //     {
        //         free(line);
        //         return (false);
        //     }
        // }
        // if (ft_strlen(line) > 0 && (heredoc->heredoc_len + ft_strlen(line) + 1 < heredoc->buffer_size))
        // {
        //     ft_strlcpy(heredoc->buffer + heredoc->heredoc_len, line, heredoc->buffer_size - heredoc->heredoc_len);
        //     ft_strlcat(heredoc->buffer, "\n", heredoc->buffer_size);
        //     heredoc->heredoc_len = heredoc->heredoc_len + ft_strlen(line) + 1;
        // }

        free(line);
        if (g_signal == SIGINT)
            return (false);
        debug("Current heredoc delimiter to match: %s", heredoc->delim[i]);
        line = readline("> ");
    }
	if (line == NULL)
			ft_write(2, "minishell: warning: here-document delimited by end-of-file\n");
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
    // char    *line;

    i = 0;
	// set_up_std_signals2();
    debug("advance to final delim");
    while (i < heredoc->delim_count)
    {
        if (!read_heredoc(heredoc, data, i))
            return (false);
        i++;
        // line = readline("> ");
        // if (g_signal == SIGINT)
        // {
        //     free (line);
        //     return (false);
        // }
        // debug("Difference between delimiter and line: %i", ft_strcmp(heredoc->delim[i], line));
        // if (line == NULL || !ft_strcmp(heredoc->delim[i], line))
        //     i++;
		// if (line == NULL)
		// 	write(2, "minishell: warning: here-document delimited by end-of-file\n", 60);
        // free (line);
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
    // heredoc->buffer = ft_calloc(heredoc->buffer_size, sizeof(char));
    // if (!heredoc->buffer)
	// {
	// 	free_heredoc(heredoc);
    //     return(false_and_print("minishell: error: heredoc memory allocation"));
	// }
    if (!advance_to_final_delim(heredoc, data))
	{
        debug("False return to process heredoc");
		free_heredoc(heredoc);
        g_signal = 0;
		return (false);
	}
    debug("True return to process heredoc");
	free_heredoc(heredoc);
    debug("Return to proecess heredoc after freeing");
    return (true);
}