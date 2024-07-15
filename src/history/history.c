/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 17:19:15 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "splash_error.h"
#include "analyser.h"
#include "splash_error.h"
#include "utils.h"

/*
Your shell should:
- Have a working history.
*/
void	load_history(t_darray *env_arr)
{
	int		fd;
	char	*path;
	char	*line;

	path = get_history_file_path(env_arr);
	fd = open(path, O_CREAT, 0644);
	free(path);
	if (fd == -1)
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
 * get the home path from the env array and if this doesnt work try
 * to get the username and create the path from that
*/
char	*get_history_file_path(t_darray *env_arr)
{
	char	*path;
	char	*home;
	char	*username;

	home = get_home(env_arr);
	if (home == NULL)
	{
		username = getenv("USER");
		if (username)
			home = ft_strjoin("/home/", username);
	}
	if (home == NULL)
		return (NULL);
	path = ft_strjoin(home, MINIHISTFILEPATH);
	free(home);
	if (path == NULL)
		stderr_and_status("history file path missing", 1);
	return (path);
}

/*
- add_history to be able to scroll through the history of commands
(readline built in)
- write line to history file for persistence
- add to history only if line is not empty
we will check if the input is a command to delete the history!
This would be
history -c or history --clear

returns true if the input is a history command! so I can skip the rest of the
readline loop~!
*/
bool	handle_history(t_data *data)
{
	sanitize_input(data->input);
	if (data->homepath == NULL)
		data->homepath = get_history_file_path(data->env_arr);
	add_to_hist_file(data->homepath, data->input);
	add_history(data->input);
	return (0);
}

/*
MINIHISTFILE
s an env var containing the path to the history file
and if it doesnt exist I will creat it. 644 are permission for the file
read only for others and W/R for the owner.
*/
bool	add_to_hist_file(const char *path, const char *input)
{
	int		fd;

	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd == -1)
		return (false);
	if (!ft_write(fd, input) || !ft_write(fd, "\n"))
	{
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}

/*
MINIHISTFILE
s an env var containing the path to the history file
and if it doesnt exist I will creat it. 644 are permission for the file
read only for others and W/R for the owner.
*/
int	clear_hist_file(const char *path)
{
	int		fd;

	if (path == NULL)
		return (stderr_and_status("history not available", 1));
	fd = open(path, O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		return (perror_and_status("open", 1));
	close(fd);
	return (0);
}
