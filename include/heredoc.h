/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:02:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 12:04:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <stdbool.h>
# include <stddef.h>
# include <libft.h>
# include "parser.h"
# include "splash.h"
# include <fcntl.h>

typedef struct s_heredoc
{
	int		delim_count;
	char	*delim[20];
	char	*file[20];
	bool	expansion[20];
}	t_heredoc;

# ifdef __cplusplus

extern "C" {
# endif

bool syntax_check_and_heredoc(t_data * data);
bool	contains_heredoc(t_list **tokenlist);
bool	execute_heredoc(t_data *data);
void	free_heredoc(t_heredoc *heredoc);
void	set_up_heredoc_signals(void);
bool	process_heredoc(t_heredoc *heredoc, t_data *data);

#  ifdef __cplusplus

}
#  endif

# endif
