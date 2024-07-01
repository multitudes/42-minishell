/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:20:28 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/01 20:52:35 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "heredoc.h"
#include "analyser.h"
#include "scanner.h"

char    *read_heredoc(char *delimiter)
{
    char    *line;
    char    *buffer;
    size_t  length_buffer;
    t_list  *tokenlist;

    line = NULL;
    buffer = ft_calloc(HEREDOC_BUFFER, sizeof(char));
    while (true)
    {
        line = readline("> ");
        while (!ft_strncmp(delimiter, line, ft_strlen(delimiter)) == 0 && ft_strlen(line) == ft_strlen(delimiter) + 1) // while delimiter not encountered
        {
            if (delimiter is unquoted)
                tokenlist = string_tokenizer(line); // create tokenlist for each line, identfying $ variables
            // perform expand_dollar on tokenlist
            // write tokenlist as string to buffer
            // copy expanded line to buffer (increase buffer if necessary)
        }
    }
    return (buffer);
}
