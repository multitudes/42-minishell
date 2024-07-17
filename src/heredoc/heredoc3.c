/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 09:41:09 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 18:33:38 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "parser.h"
#include "executer.h"
#include "splash_error.h"
#include "utils.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static void	sigint_handler2(int sig)
{
	if (sig == SIGINT)
	{
		ft_write(1, "^C");
		ioctl(0, TIOCSTI, "\n");
		g_signal = sig;
	}
	return ;
}

void	set_up_heredoc_signals(void)
{
	rl_catch_signals = 0;
	if (signal(SIGINT, sigint_handler2) == SIG_ERR)
		return (perror("splash: SIG_ERR"));
	return ;
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

bool	save_heredoc_delimiter(t_list *tokenlist, t_heredoc *heredoc)
{
	remove_leading_backslash((t_token *)(tokenlist->next->content));
	heredoc->delim[heredoc->delim_count] = \
		ft_strdup(get_token_lexeme(tokenlist->next));
	if (!heredoc->delim[heredoc->delim_count])
	{
		free_heredoc(heredoc);
		return (stderr_and_bool("heredoc memory error", false));
	}
	return (true);
}
