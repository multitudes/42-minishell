/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 02:19:34 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/04 02:19:39 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "heredoc.h"
#include "analyser.h"
#include "scanner.h"
#include "builtins.h"
#include "utils.h"
#include "error.h"
#include <libft.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
Frees the memory used for storing delimiter lexemes.
*/
void    free_heredoc(t_heredoc *heredoc)
{
    int i;

    i = 0;
    while (i < heredoc->delim_count)
        free(heredoc->delim[i++]);
}

/*
Increases buffer for heredoc if addition of next line would surpass buffer size.
*/
static int increase_heredoc_size(t_heredoc *heredoc)
{
    char    *new_buffer;

    new_buffer = ft_calloc(heredoc->buffer_size + HEREDOC_BUFFER, sizeof(char));
    if (!new_buffer)
        return((int)print_error_status("minishell: error: heredoc memory allocation", 1));
    heredoc->buffer_size = heredoc->buffer_size + HEREDOC_BUFFER;
    ft_strlcpy(new_buffer, heredoc->buffer, heredoc->buffer_size);
    free(heredoc->buffer);
    heredoc->buffer = new_buffer;
    return (0);
}

/*
Read and save content of final heredoc.
*/
static int read_heredoc(t_heredoc *heredoc, t_data *data, int i)
{
    char    *line;

    debug("read heredoc");
    line = readline("> ");
    while (line && (ft_strncmp(heredoc->delim[i], line, ft_strlen(heredoc->delim[i])) != 0 && ft_strlen(line) != ft_strlen(heredoc->delim[i]) - 1)) // while delimiter not encountered
    {
        if (line && heredoc->expansion[i] == true && ft_strchr(line, '$'))
            line = replace_dollar_vars(data->env_arr, line);
        while (heredoc->heredoc_len + ft_strlen(line) + 1 >= heredoc->buffer_size)
        {
            if (increase_heredoc_size(heredoc))
            {
                free(line);
                return (1);
            }
        }
        if (ft_strlen(line) > 0 && (heredoc->heredoc_len + ft_strlen(line) + 1 < heredoc->buffer_size))
        {
            ft_strlcpy(heredoc->buffer + heredoc->heredoc_len, line, heredoc->buffer_size - heredoc->heredoc_len);
            ft_strlcat(heredoc->buffer, "\n", heredoc->buffer_size);
            heredoc->heredoc_len = heredoc->heredoc_len + ft_strlen(line) + 1;
        }
        free(line);
        line = readline("> ");
    }
    free(line);
    return (0);
}

/*
Prompt for each delimiter and advancing to next heredoc/delimiter.
*/
static void advance_to_final_delim(t_heredoc *heredoc, int *i)
{
    char    *line;

    debug("advance to final delim");
    while (heredoc->delim_count > 1 && *i < heredoc->delim_count - 1)
    {
        debug("Current delimiter to match: %s", heredoc->delim[*i]);
        line = readline("> ");
        if (line && !(ft_strncmp(heredoc->delim[*i], line, ft_strlen(heredoc->delim[*i])) != 0 && ft_strlen(line) != ft_strlen(heredoc->delim[*i]) - 1))
            (*i)++;
        free (line);
    }
}

/*
Reads heredoc content. Only input for final delimiter
gets saved and passed to stdin of any commands.
*/
int process_heredoc(t_heredoc *heredoc, t_data *data)
{
    int     i;
    int     status;

    status = 0;
    heredoc->buffer = ft_calloc(heredoc->buffer_size, sizeof(char));
    if (!heredoc->buffer)
        return((int)print_error_status("minishell: error: heredoc memory allocation", 1));
    i = 0;
    advance_to_final_delim(heredoc, &i);
    status = read_heredoc(heredoc, data, i);
    free_heredoc(heredoc);
    return (status);
}