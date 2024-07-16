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

/*
Set up heredoc(s). Up to 20 heredocs/delimiters are supported.
TODO Add syntax check on tokenlist.
*/
static bool	init_heredoc(t_list *tokenlist, t_heredoc *heredoc)
{
	heredoc->delim_count = 0;
	while (tokenlist)
	{
		if (is_heredoc_token(get_token_type(tokenlist)))
		{
			if (is_heredoc_delim(tokenlist->next))
			{
				if (!save_heredoc_delimiter(tokenlist, heredoc))
					return (false);
				tokenlist = tokenlist->next;
				if (!create_heredoc_file(tokenlist, heredoc))
					return (false);
				heredoc->delim_count++;
			}
			else
				return (stderr_and_bool(\
						"syntax error: unexpected heredoc token", false));
		}
		tokenlist = tokenlist->next;
	}
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
Checks for tokenlist syntax and heredoc token.
*/
static bool	is_heredoc_and_syntax_check(t_list **tokenlist)
{
	t_tokentype	tokentype;
	bool		contains_heredoc;
	t_list		*curr;
	t_list		*tmp;

	tmp = NULL;
	contains_heredoc = false;
	curr = *tokenlist;
	while (curr)
	{
		tokentype = get_token_type(curr);
		if (is_heredoc_token(tokentype))
			contains_heredoc = true;
		else if (tokentype == COMMENT)
		{
			tmp = (curr)->prev;
			ft_lstclear(&curr, free_tokennode);
			if (tmp)
				tmp->next = NULL;
			else
				*tokenlist = NULL;
			break ;
		}
		curr = curr->next;
	}
	return (contains_heredoc);
}

bool	syntax_check_and_heredoc(t_data *data)
{
	t_list	*tokenlist;

	tokenlist = data->tokenlist;
	if (is_heredoc_and_syntax_check(&tokenlist))
	{
		data->tokenlist = tokenlist;
		return (execute_heredoc(data));
	}
	else
		data->tokenlist = tokenlist;
	return (true);
}
