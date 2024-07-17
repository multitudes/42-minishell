/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:48:30 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/15 16:32:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "parser.h"
#include "splash_error.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "fd.h"

static void	upd_status_redir(int fd, int fd2, char *file, uint8_t *status)
{
	if (dup2(fd, fd2) < 0)
		*status = perror_and_status(file, 1);
}

uint8_t	dup2_by_redirect_type(t_tokentype type, \
					char *filename, int fd, uint8_t *status)
{
	*status = 0;
	if (type == REDIRECT_OUT || type == DGREAT || type == CLOBBER)
		upd_status_redir(fd, STDOUT_FILENO, filename, status);
	else if (type == REDIRECT_IN || type == DLESS || type == DLESSDASH)
		upd_status_redir(fd, STDIN_FILENO, filename, status);
	else if (type == REDIRECT_BOTH || type == REDIRECT_BOTH_APP \
			|| type == GREATAND)
	{
		upd_status_redir(fd, STDOUT_FILENO, filename, status);
		upd_status_redir(fd, STDERR_FILENO, filename, status);
	}
	return (*status);
}

int	open_fd_by_redirect_type(t_tokentype type, char *filename, uint8_t *status)
{
	int	fd;

	fd = -1;
	if (type == REDIRECT_OUT || type == REDIRECT_BOTH || type == GREATAND || \
			type == CLOBBER)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | \
							S_IWUSR | S_IRGRP | S_IROTH);
	else if (type == REDIRECT_IN || type == DLESS || type == DLESSDASH)
		fd = open(filename, O_RDONLY);
	else if (type == REDIRECT_BOTH_APP || type == DGREAT)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | \
							S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		*status = perror_and_status(filename, 1);
	return (fd);
}

uint8_t	iterate_tokenlist_for_redirections(t_ast_node **ast, \
											t_list **tokenlist, \
											int *token_counter)
{
	uint8_t		status;

	status = 0;
	while (*tokenlist)
	{
		if (supported_redirect_token(get_token_type(*tokenlist)))
		{
			status = setup_redirect(tokenlist, get_token_type(*tokenlist));
			if (*token_counter == 0)
				(*ast)->tokenlist = *tokenlist;
			if (status != 0)
				return (status);
			continue ;
		}
		else
			(*token_counter)++;
		*tokenlist = (*tokenlist)->next;
	}
	return (status);
}

/*
Execute commands that contain basic redirection:
'<', '>', '>>' and also heredocs << after preprocessing.
Heredocs are handled similar to stdin redirection.
*/
uint8_t	execute_redirection(t_ast_node **ast)
{
	uint8_t		status;
	t_list		*tokenlist;
	int			token_counter;

	status = 0;
	token_counter = 0;
	tokenlist = (*ast)->tokenlist;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
	status = iterate_tokenlist_for_redirections(ast, &tokenlist, \
												&token_counter);
	if (tokenlist == NULL && token_counter == 0)
	{
		(*ast)->tokenlist = NULL;
		(*ast)->type = NODE_NULL;
	}
	return (status);
}
