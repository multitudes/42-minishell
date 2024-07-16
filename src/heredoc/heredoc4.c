/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:47:45 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/16 16:47:48 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "scanner.h"
#include "utils.h"
#include "splash_error.h"
#include <libft.h>

/*
Removes all single and double qoutes from a string.
*/
static void	remove_quotes(char *string)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (string && string[i])
	{
		if (string[i] != '"' && string[i] != '\'')
		{
			string[j] = string[i];
			j++;
		}
		i++;
	}
	string[j] = '\0';
}

/*
Checks for and handles single/double quotes in heredoc delimiter.
TODO handle inner quotes like bash
*/
bool	process_delim_quotes(t_heredoc *heredoc)
{
	int	i;
	int	count_single_quotes;
	int	count_double_quotes;

	i = 0;
	count_single_quotes = 0;
	count_double_quotes = 0;
	while (i < heredoc->delim_count)
	{
		heredoc->expansion[i] = true;
		count_single_quotes = count_char_in_str(heredoc->delim[i], '\'');
		count_double_quotes = count_char_in_str(heredoc->delim[i], '"');
		if (count_single_quotes % 2 || count_double_quotes % 2)
			return (stderr_and_bool("heredoc: invalid delimiter", false));
		else if (count_single_quotes > 1 || count_double_quotes > 1)
		{
			remove_quotes(heredoc->delim[i]);
			heredoc->expansion[i] = false;
		}
		i++;
		count_single_quotes = 0;
		count_double_quotes = 0;
	}
	return (true);
}

/*
Allocated a filename in the format heredoc_i.
Next available filename up to i = 19 is provided.
Then allocations starts again from beginning.
*/
static char	*get_heredoc_filename(void)
{
	static int	i = 0;
	char		*index;
	char		*new_filename;

	if (i == 19)
		i = 0;
	index = ft_itoa(i++);
	if (!index)
		return (perror_and_null("heredoc set up"));
	new_filename = ft_strjoin(".tmp/heredoc_", index);
	if (!new_filename)
		return (perror_and_null("heredoc set up"));
	free(index);
	return (new_filename);
}

/*
Replaces heredoc delimiter in tokenlist
with newly provided filename for storing heredoc contents.
Also saves filename to heredoc struct.
*/
bool	create_heredoc_file(t_list *tokenlist, t_heredoc *heredoc)
{
	char	*temp;

	temp = ((t_token *)(tokenlist->content))->lexeme;
	((t_token *)(tokenlist->content))->lexeme = get_heredoc_filename();
	free(temp);
	if (!get_token_lexeme(tokenlist))
	{
		free_heredoc(heredoc);
		return (stderr_and_bool("heredoc setup error", false));
	}
	heredoc->file[heredoc->delim_count] = \
		ft_strdup(((t_token *)(tokenlist->content))->lexeme);
	((t_token *)(tokenlist->content))->type = HEREDOC_FILE;
	return (true);
}

/*
Frees the memory used for storing delimiter lexemes.
*/
void	free_heredoc(t_heredoc *heredoc)
{
	int	i;

	i = 0;
	while (i < heredoc->delim_count)
	{
		free(heredoc->delim[i]);
		free(heredoc->file[i++]);
	}
}
