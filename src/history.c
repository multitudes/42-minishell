/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/05 14:09:26 by lbrusa           ###   ########.fr       */
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
	// debug("file path %s \n", path);
	if (path == NULL)
		path = "~/minihistfile";
	fd = open(path, O_CREAT, 0644);
	if (fd == -1)
	{
		perror("open ----------- ");
		return ;
	}
	line = get_next_line(fd);
	// debug("line :%s\n", line);
	while (line != NULL)
	{
		line[ft_strlen(line) - 1] = '\0';
		// debug("line :%s\n", line);
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
	return (path = ft_strjoin(home, MINIHISTFILEPATH));
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
but I will check it later
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
*/
bool	handle_history(t_mini_data *data)
{
	// if (data->input == NULL || ft_strlen(data->input) <= 0 || (ft_strlen(data->input) == 1 \
	// && (data->input[0] < 32 || data->input[0] > 126)))
	// 	return (0);
	if (ft_strncmp(data->input, "history -c", 11) == 0 || ft_strncmp(data->input, "history --clear", 16) == 0)
	{
		debug("clearing history\n");
		clear_hist_file();
		rl_clear_history();
		return (0);
	}
	else if (ft_strncmp(data->input, "history", 7) == 0)
	{
		print_history();
		return (0);
	}
	if (add_to_hist_file(data->input))
	{
		perror("add_to_hist_file");
		return (0);
	}
	add_history(data->input);
	// need debugging
	if (update_env(data, "_", data->input) == FALSE)
	{
		perror("update_env for _ with history input");
		return (0);
	}
	return (TRUE);
}

/*
MINIHISTFILE
s an env var containing the path to the history file
and if it doesnt exist I will creat it. 644 are permission for the file
read only for others and W/R for the owner.
*/
int add_to_hist_file(const char *input)
{
	int fd;
	char *path;

	path = get_history_file_path();
	if (path == NULL)
	{
		perror("get_history_file_path");
		return (1);
	}
	fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	if (write(fd, input, ft_strlen(input)) == -1 || write(fd, "\n", 1) == -1)
	{
		perror("write");
		return (1);
	}
	// debug("written input: %s to %s", input, path);
	close(fd);
	return (0);
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
	if (path == NULL)
	{
		perror("get_history_file_path");
		return ;
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	line = get_next_line(fd);
	// debug("printing history\n");
	while (line != NULL)
	{
		debug("%d: %s", i, line);
		free(line);
		i++;
		line = get_next_line(fd);
	}
	printf("%d",i);
	close(fd);
}
