/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 13:00:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
Your shell should:
- Have a working history.
*/
void load_history(void)
{
	int fd;
	char *path;
	char *line;

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

*/
char	*get_history_file_path(void)
{
	char *path;
	char *home;

	home = getenv("HOME");
	if (home == NULL || home[0] == '\0')
	{
		perror("getenv");
		debug("HOME env var not set\n");
		return (NULL);
	}
	
	// debug("file path %s \n", path);
	path = ft_strjoin(home, MINIHISTFILEPATH);
	if (path == NULL)
		path = "~/minihistfile";
	return (path);
}

/*
util . otherwise non ascii chars like ∂ will wreac havoc and the read function
will stop reading the file.
inline functions are typically defined in header files (cant do it here)
*/
inline int ft_isascii(const int c) 
{
    return (c >= 0 && c <= 127);
}

/*
The char input is sanitized by removing all non ascii characters
andn it is coming from a dynamic memory allocation so I can 
overwrite it- interestingly the compiler did not complain about it
being const maybe because the pointer is const... but the contents
can vary? 
*/
void	sanitize_input(const char *input)
{
	int i;
	char *tmp;

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
	if (!add_to_hist_file(data->input))
		perror("add_to_hist_file");
	add_history(data->input);
	// need debugging?
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
	int fd;
	char *path;

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
int clear_hist_file()
{
	int fd;
	char *path;

	path = get_history_file_path();
	if (path == NULL)
	{
		perror("get_history_file_path");
		return (1);
	}
	fd = open(path, O_WRONLY | O_TRUNC);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	debug("removed history\n");
	close(fd);
	return (0);
}

void	print_history(void)
{
	int i;
	int fd;
	char *path;
	char *line;

	i = 0;
	path = get_history_file_path();
	fd = open(path, O_RDONLY);
	free(path);
	if (fd == -1)
	{
		perror("open");
		return ;
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
}
