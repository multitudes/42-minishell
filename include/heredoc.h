/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:02:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/11 07:50:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <stdbool.h>
# include <stddef.h>
# include <libft.h>
# include "parser.h"
# include "minishell.h"
# include <fcntl.h>

// # define HEREDOC_BUFFER 1024

typedef struct s_heredoc
{
	int		delim_count;
	char	*delim[20];
	char	*file[20];
	bool	expansion[20];
	// char	*buffer;
	// size_t	buffer_size;
	// size_t	heredoc_len;
}	t_heredoc;

# ifdef __cplusplus

extern "C" {
# endif

bool	set_up_heredocs(t_data *data);
bool	contains_heredoc(t_list *tokenlist);
bool	execute_heredoc(t_data *data);
void	free_heredoc(t_heredoc *heredoc);
void	set_up_heredoc_signals(void);
bool	process_heredoc(t_heredoc *heredoc, t_data *data);
// int		redirect_and_execute_heredoc(t_ast_node *ast, t_data *data,
// 										t_heredoc *heredoc);

#  ifdef __cplusplus

}
#  endif

# endif
