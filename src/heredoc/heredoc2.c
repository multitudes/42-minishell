/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 02:19:34 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/05 19:36:57 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <signal.h>

// static void	sigint_handler2(int sig)
// {
//     if (sig == SIGINT)
//     {	
// 		g_signal = sig;
//         write(1, "testing signal\n", 1);

//     }
// 	else
// 		g_signal = sig;
// 	return ;
// }

// int	set_up_signals2(void)
// {
// 	if ((signal(SIGINT, sigint_handler2) == SIG_ERR))
// 		return (status_and_perror("SIG_ERR signal failed", 1));
// 	return (0);
// }

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
static bool increase_heredoc_size(t_heredoc *heredoc, int len)
{
    char    *new_buffer;

    heredoc->buffer_size = heredoc->buffer_size + len + HEREDOC_BUFFER;
    new_buffer = ft_calloc(heredoc->buffer_size, sizeof(char));
    if (!new_buffer)
        return(false_and_print("minishell: error: heredoc memory allocation"));
    ft_strlcpy(new_buffer, heredoc->buffer, heredoc->buffer_size);
    free(heredoc->buffer);
    heredoc->buffer = new_buffer;
    return (true);
}

/*
Read and save content of final heredoc.
*/
static bool read_heredoc(t_heredoc *heredoc, t_data *data, int i)
{
    char    *line;
    char    *temp;

    debug("read heredoc");
    line = readline("> ");
    while (line && ft_strcmp(heredoc->delim[i], line))
    {
        if (line && heredoc->expansion[i] == true && ft_strchr(line, '$'))
        {
            temp = replace_dollar_vars(data->env_arr, line);
            free(line);
            line = temp;
        }
        if (heredoc->heredoc_len + ft_strlen(line) + 1 >= heredoc->buffer_size)
        {
            if (!increase_heredoc_size(heredoc, (int)ft_strlen(line)))
            {
                free(line);
                return (false);
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
	if (line == NULL)
			write(2, "minishell: warning: here-document delimited by end-of-file\n", 60);
    free(line);
    return (true);
}

/*
Prompt for each delimiter and advancing to next heredoc/delimiter.
*/
static void advance_to_final_delim(t_heredoc *heredoc, int *i)
{
    char    *line;
	// set_up_signals2();
    debug("advance to final delim");
    while (*i < heredoc->delim_count - 1)
    {
        debug("Current delimiter to match: %s", heredoc->delim[*i]);
        line = readline("> ");
        debug("Difference between delimiter and line: %i", ft_strcmp(heredoc->delim[*i], line));
        if (line == NULL || !ft_strcmp(heredoc->delim[*i], line))
            (*i)++;
		if (line == NULL)
			write(2, "minishell: warning: here-document delimited by end-of-file\n", 60);
        free (line);
    }
}

/*
Reads heredoc content. Only input for final delimiter
gets saved and passed to stdin of any commands.
*/
bool	process_heredoc(t_heredoc *heredoc, t_data *data)
{
    int     i;

    debug("Process heredoc");
    heredoc->buffer = ft_calloc(heredoc->buffer_size, sizeof(char));
    if (!heredoc->buffer)
	{
		free_heredoc(heredoc);
        return(false_and_print("minishell: error: heredoc memory allocation"));
	}
    i = 0;
    advance_to_final_delim(heredoc, &i);
    if (!read_heredoc(heredoc, data, i))
	{
		free_heredoc(heredoc);
		free(heredoc->buffer);
		return (false_and_print("minishell: error: heredoc memory allocation"));
	}
	free_heredoc(heredoc);
    return (true);
}