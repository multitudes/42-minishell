/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 11:46:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_history(t_darray *env_arr)
{
	int		i;
	int		fd;
	char	*path;
	char	*line;

	i = 0;
	path = get_history_file_path(env_arr);
	fd = open(path, O_RDONLY);
	free(path);
	if (fd == -1)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
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

/*
The char input is sanitized by removing all non ascii characters
andn it is coming from a dynamic memory allocation so I can 
overwrite it- interestingly the compiler did not complain about it
being const maybe because the pointer is const... but the contents
can vary? 
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
