/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 21:02:07 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 13:49:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <stdbool.h>
# include <stddef.h>
# include "libft.h"
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
bool	save_heredoc_delimiter(t_list *tokenlist, t_heredoc *heredoc);
bool	create_heredoc_file(t_list *tokenlist, t_heredoc *heredoc);
bool	process_delim_quotes(t_heredoc *heredoc);
char	*expand_heredoc(bool expand, t_data *data, char *line);
bool	execute_heredoc(t_data *data);
void	free_heredoc(t_heredoc *heredoc);
void	set_up_heredoc_signals(void);
bool	process_heredoc(t_heredoc *heredoc, t_data *data);

#  ifdef __cplusplus

}
#  endif

# endif
