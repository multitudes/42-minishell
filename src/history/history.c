/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/22 15:22:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (fd == -1)
	{
		perror("open ----------- ");
		return ;
	}
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
The function `get_history_file_path` generates the path for the history file. 
The path is constructed by joining the HOME environment variable and the MINIHISTFILEPATH. 
MINIHISTFILEPATH is a predefined macro that could be passed as a parameter 
to the program during compilation as a flag. 
However, there are several scenarios where the history file might not exist:
- The MINIHISTFILEPATH macro could be unset, similar to the BUFFER in get_next_line.
- The HOME environment variable might be unset.
In case of any failure in obtaining these values, a fallback path is returned.
*/
char	*get_history_file_path(void)
{
	char	*path;
	char	*home;

	home = getenv("HOME");
	path = ft_strjoin(home, MINIHISTFILEPATH);
	if (path == NULL || home == NULL || *home == '\0')
		path = ft_strdup("~/.minishell_history");
	return (path);
}

/*
The function `add_history` allows scrolling through the 
history of commands (a feature built into readline). 
It writes each line to the history file for persistence. 
Only non-empty lines are added to the history. 
Additionally, the function checks if the input is a command to clear 
the history.
Such commands could be 'history -c' or 'history --clear'.

returns true if the input is a history command! so I can skip the 
rest of the readline loop~!
*/
bool	handle_history(t_data *data)
{
	if (!add_to_hist_file(data->input))
		perror("add_to_hist_file");
	add_history(data->input);
	if (update_env(data, "_", data->input) == FALSE)
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
	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	free(path);
	if (fd == -1)
	{
		perror("open");
		return (false);
	}
	if (write(fd, input, ft_strlen(input)) == -1 || write(fd, "\n", 1) == -1)
	{
		close(fd);
		perror("write");
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
