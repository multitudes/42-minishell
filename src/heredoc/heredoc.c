/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:20:28 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/14 21:16:12 by lbrusa           ###   ########.fr       */
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
#include <readline/history.h> // needed?

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
removes leading backslash in lexeme (used to sanitize heredoc delimiters)
*/
static void	remove_leading_backslash(t_token *token)
{
	char	*temp;
	char	*lexeme;

	lexeme = token->lexeme;
	if (lexeme && lexeme[0] == '\\' && (lexeme[1] != '\0'))
	{
		temp = ft_strdup(lexeme + 1);
		free(token->lexeme);
		token->lexeme = temp;
	}
}

/*
removes all single and double qoutes from a string.
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
static bool	process_delim_quotes(t_heredoc *heredoc)
{
	int	count_single_quotes;
	int	count_double_quotes;
	int	i;

	count_single_quotes = 0;
	count_double_quotes = 0;
	i = 0;
	while (i < heredoc->delim_count)
	{
		count_single_quotes = count_char_in_str(heredoc->delim[i], '\'');
		count_double_quotes = count_char_in_str(heredoc->delim[i], '"');
		if (count_single_quotes % 2 || count_double_quotes % 2)
			return (stderr_and_bool("heredoc: invalid delimiter", false));
		else if (count_single_quotes > 1 || count_double_quotes > 1)
		{
			remove_quotes(heredoc->delim[i]);
			heredoc->expansion[i] = false;
		}
		else
			heredoc->expansion[i] = true;
		i++;
		count_single_quotes = 0;
		count_double_quotes = 0;
	}
	i = 0;
	return (true);
}

/*
Set up heredoc(s). Up to 20 heredocs/delimiters are supported.
TODO Add syntax check on tokenlist.
*/
static bool	init_heredoc(t_list *tokenlist, t_heredoc *heredoc)
{
	char	*temp;

	heredoc->delim_count = 0;
	while (tokenlist)
	{
		if (is_heredoc_token(get_token_type(tokenlist)))
		{
			if (is_heredoc_delim(tokenlist->next))
			{
				remove_leading_backslash((t_token *)(tokenlist->next->content));
				heredoc->delim[heredoc->delim_count] = ft_strdup(get_token_lexeme(tokenlist->next));
				if (!heredoc->delim[heredoc->delim_count])
				{
					free_heredoc(heredoc);
					return (stderr_and_bool("heredoc memory error", false));
				}
				debug("Set delimiter %i: %s", heredoc->delim_count, heredoc->delim[heredoc->delim_count]);
				tokenlist = tokenlist->next;
				temp = ((t_token *)(tokenlist->content))->lexeme;
				((t_token *)(tokenlist->content))->lexeme = get_heredoc_filename();
				free(temp);
				if (!get_token_lexeme(tokenlist))
				{
					free_heredoc(heredoc);
					return (stderr_and_bool("heredoc setup error", false));
				}
				heredoc->file[heredoc->delim_count] = ft_strdup(((t_token *)(tokenlist->content))->lexeme);
				((t_token *)(tokenlist->content))->type = HEREDOC_FILE;
				heredoc->delim_count++;
			}
			else
				return (stderr_and_bool("syntax error: unexpected heredoc token", false));
		}
		tokenlist = tokenlist->next;
	}
	debug("init heredoc complete, delimiter count: %i", heredoc->delim_count);
	return (true);
}

/*
TBC
Executes heredoc and then passes heredoc to stdin of system command
or calls builtin functions depending on ast node type.
*/
bool	execute_heredoc(t_data *data)
{
	t_heredoc	heredoc;

	debug("execute heredoc");
	set_up_heredoc_signals();
	if (!init_heredoc(data->tokenlist, &heredoc) \
		|| !process_delim_quotes(&heredoc) || !process_heredoc(&heredoc, data))
	{
		set_up_std_signals();
		return (false);
	}
	set_up_std_signals();
	return (true);
}

/*
Checks for heredoc token in tokenlist.
CONSIDER TODO valid_syntax_and_heredoc()
*/
bool	contains_heredoc(t_list **tokenlist)
{
	t_tokentype	tokentype;
	bool		contains_heredoc;
	t_list		*curr;
	t_list		*tmp;

	tmp = NULL;
	contains_heredoc = false;
	curr = tokenlist;
	while (curr)
	{
		tokentype = get_token_type(curr);
		if (is_heredoc_token(tokentype))
			contains_heredoc = true;
		else if (tokentype == COMMENT)
		{
			debug("comment token found");
			tmp = (curr)->prev;
			ft_lstclear(&curr, free_tokennode);
			debug("tokenlist is freed");
			if (tmp)
				tmp->next = NULL;
			else
				*tokenlist = NULL;
			break ;
		}
		curr = curr->next;
	}
	// debug("tmp is %s", get_token_lexeme(tmp));
	// debug("head is %s", get_token_lexeme(get_head(tmp)));
	// debug("tokenlist head is %s", get_token_lexeme((*tokenlist)));
	return (contains_heredoc);
}

bool	syntax_check_and_heredoc(t_data *data)
{
	t_list	*tokenlist;

	tokenlist = data->tokenlist;
	print_tokenlist(tokenlist);
	if (contains_heredoc(&tokenlist))
	{
		data->tokenlist = tokenlist;
		return (execute_heredoc(data));
	}
	else
	{
		debug("no heredoc found");
		debug("tokenlist head is %s", get_token_lexeme(tokenlist));
		data->tokenlist = tokenlist;
	}
	debug("no heredoc found");
	return (true);
}
