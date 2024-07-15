/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:31:05 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 16:24:09 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executer.h"
#include "splash.h"
#include "splash_error.h"

/*
 * Opens and duplicates file used for redirection.
 * Deletes redirection and filename token.
 * Deletes files that were created for heredocs.
 */
uint8_t	setup_redirect(t_list **tokenlist, t_tokentype type)
{
	char	*filename;
	int		fd;
	uint8_t	status;

	debug("setup redirect");
	fd = -1;
	status = 0;
	consume_token_and_connect(tokenlist);
	if (!(*tokenlist))
		return (stderr_and_status("redirect syntax error", 2));
	filename = get_token_lexeme(*tokenlist);
	fd = open_fd_by_redirect_type(type, filename, &status);
	if (status != 0)
		return (status);
	if (dup2_by_redirect_type(type, filename, fd, &status) != 0)
	{
		close(fd);
		return (status);
	}
	if (is_heredoc_token(type))
		unlink(filename);
	consume_token_and_connect(tokenlist);
	close(fd);
	return (0);
}

bool	supported_redirect_token(t_tokentype type)
{
	if (type == REDIRECT_IN || type == DLESS || type == DLESSDASH)
		return (true);
	else if (type == REDIRECT_OUT || type == DGREAT)
		return (true);
	else if (type == GREATAND || type == CLOBBER)
		return (true);
	else if (type == REDIRECT_BOTH || type == REDIRECT_BOTH_APP)
		return (true);
	return (false);
}

/*
when I need to free a string array like the envpaths
*/
int	free_array(char **envpaths)
{
	int	i;

	i = 0;
	while (envpaths[i])
	{
		free(envpaths[i]);
		i++;
	}
	free(envpaths);
	return (0);
}
