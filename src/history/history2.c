/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 13:32:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "splash_error.h"

int	print_history(void)
{
	int		i;
	int		fd;
	char	*line;

	i = 0;
	fd = open(HIST_FILE, O_RDONLY);
	if (fd == -1)
		return (perror_and_status("print history", EXIT_FAILURE));
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%5d  %s", i, line);
		free(line);
		i++;
		line = get_next_line(fd);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

/**
 * The char input is sanitized by removing all non ascii characters
 */
bool	sanitize_input(const char *input)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = (char *)input;
	while (input[i])
	{
		if (!ft_isascii(input[i]))
			i++;
		else
			*tmp++ = input[i++];
	}
	*tmp = '\0';
	return (true);
}
