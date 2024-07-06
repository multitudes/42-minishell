/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/06 15:30:10 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "splash_error.h"
#include "analyser.h"
#include "splash_error.h"

/*
Your shell should:
- Have a working history.
*/
void	load_history(void)
{
	int		fd;
	char	*path;
	char	*line;

	path = get_history_file_path();
	fd = open(path, O_CREAT, 0644);
	free(path);
	if (fd == -1 && status_and_perror("open", 1))
		return ;
	line = get_next_line(fd);
	while (line != NULL)
	{
		line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

/*

*/
char	*get_history_file_path(void)
{
	char	*path;
	char	*home;

	home = get_home(NULL);
	if (home == NULL || home[0] == '\0')
		return (NULL);
	debug("history home: %s", home);
	path = ft_strjoin(home, MINIHISTFILEPATH);
	debug("path: %s", path);
	if (path == NULL)
		path = ft_strdup("~/.minishell_history");
	return (path);
}

/*
- add_history to be able to scroll through the history of commands
(readline built in)
- write line to history file for persistence
-  add to history only if line is not empty
we will check if the input is a command to delete the history!
This would be
history -c or history --clear

returns true if the input is a history command! so I can skip the rest of the
readline loop~!
*/
bool	handle_history(t_data *data)
{
	sanitize_input(data->input);
	if (!add_to_hist_file(data->input))
		return (false);
	add_history(data->input);
	if (update_env(data->env_arr, "_", data->input) == FALSE)
		perror("update_env for _ with history input");
	return (0);
}

/*
MINIHISTFILE
s an env var containing the path to the history file
and if it doesnt exist I will creat it. 644 are permission for the file
read only for others and W/R for the owner.
*/
bool	add_to_hist_file(const char *input)
{
	int		fd;
	char	*path;

	path = get_history_file_path();
	if (path == NULL)
		return (false);
	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd == -1)
	{
		free(path);
		return (false);
	}
	free(path);
	if (fd == -1)
		return (false);
	if (write(fd, input, ft_strlen(input)) == -1 || write(fd, "\n", 1) == -1)
	{
		close(fd);
		return (false);
	}
	close(fd);
	debug("added to history: %s", input);
	return (true);
}

/*
MINIHISTFILE
s an env var containing the path to the history file
and if it doesnt exist I will creat it. 644 are permission for the file
read only for others and W/R for the owner.
*/
int	clear_hist_file(void)
{
	int		fd;
	char	*path;

	path = get_history_file_path();
	fd = open(path, O_WRONLY | O_TRUNC);
	free(path);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	close(fd);
	return (0);
}
