/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:02:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/01 21:07:06 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# ifdef __cplusplus

extern "C" {
# endif

# define HEREDOC_BUFFER 1024

typedef struct  s_heredoc
{
    int     delim_count;
    char    *delim[10];
    bool    expansion[10];
    char    *buffer;
    size_t  buffer_size;
    size_t  heredoc_len;
} t_heredoc;

int     execute_heredoc(t_ast_node *ast, t_data *data);
void    free_heredoc(t_heredoc *heredoc);
bool    is_heredoc(t_list *tokenlist);
int     process_heredoc(t_heredoc *heredoc, t_data *data);
int     redirect_and_execute_heredoc(t_ast_node *ast, t_data *data, t_heredoc *heredoc);

# ifdef __cplusplus

}
# endif

# endif
