/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:36:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/09 14:38:05 by lbrusa           ###   ########.fr       */
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
this is different than the get_home function because it is called before we create 
our minienv array. So we cannot use the mini_get_env function. 
*/
char	*get_history_file_path(void)
{
	char	*path;
	char	*home;

	home = getenv("HOME");
	debug("HOME from bash env: %s", home);
	if (home)
		home = ft_strdup(home);
	else
	{
		debug("HOME not set in env, trying to get from /etc/passwd");
        char *username = getenv("USER");
		debug("Username: %s", username);
		if (username)
			home = ft_strjoin("/home/", username);
		else
			home = ft_strdup("/home");
    }
	if (home == NULL)
		return (NULL);
	debug("history home: %s", home);
	path = ft_strjoin(home, MINIHISTFILEPATH);
	debug("path: %s", path);
	if (path == NULL)
		print_minishell_error_status("history file path missing", 1);
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
	if (!add_to_hist_file(data->input))
		return (false);
	add_history(data->input);
	if (update_env(data->env_arr, "_", "") == FALSE)
		perror("in update_env for _ ");
	debug("env _ is reset to -%s-", mini_get_env(data->env_arr, "_"));
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


/*
TODO implement this for macos and do the same for linux if we have time
also implement the get_home function for linux
and replace the popen with our commands
*/
// char *get_macos_home_directory(void) {
//     int fd;
//     char buffer[1035];
//     ssize_t numRead;
//     char *home = NULL;

//     /* Open the command for reading. */
//     FILE *fp = popen("dscl . -read /Users/$(whoami) NFSHomeDirectory | awk '{print $2}'", "r");
//     if (fp == NULL) {
//         printf("Failed to run command\n");
//         exit(1);
//     }

//     /* Convert FILE* stream to file descriptor */
//     fd = fileno(fp);
//     if (fd == -1) {
//         perror("fileno failed");
//         pclose(fp);
//         return NULL;
//     }

//     /* Initialize buffer */
//     memset(buffer, 0, sizeof(buffer));

//     /* Read the output a chunk at a time - output it. */
//     numRead = read(fd, buffer, sizeof(buffer) - 1); // Read up to sizeof(buffer)-1 bytes
//     if (numRead > 0) {
//         // Ensure null-termination
//         buffer[numRead] = '\0';

//         // Remove newline character at the end if present
//         if (buffer[numRead - 1] == '\n') {
//             buffer[numRead - 1] = '\0';
//         }

//         home = strdup(buffer);
//     } else {
//         printf("Failed to read data\n");
//     }

//     /* Close */
//     pclose(fp);

//     return home;
// }