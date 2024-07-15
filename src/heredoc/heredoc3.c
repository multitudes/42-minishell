/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 09:41:09 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/11 08:33:28 by lbrusa           ###   ########.fr       */
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
		return (perror("minishell: SIG_ERR"));
	return ;
}
