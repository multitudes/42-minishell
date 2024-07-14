/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/14 17:31:06 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globbing.h"
#include <libft.h>
#include "debug.h"
#include "utils.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "splash_error.h"
#include "parser.h"

/*
 * This function will initialize the struct used in globbing func
 * it will return true if it was successful
*/
bool	init(t_globbing *gl)
{
	gl->dirp = opendir(".");
	if (!gl->dirp)
		return (false_and_perr("opendir"));
	errno = 0;
	gl->dir_entry = readdir(gl->dirp);
	if (!gl->dir_entry && errno)
		return (false_and_perr("readdir"));
	gl->full_path = NULL;
	if (getcwd(gl->cwd, PATH_MAX) == NULL)
		return (false_and_perr("getcwd"));
	return (true);
}

/*
 * This function will loop through the directory and check 
 * if the files match the pattern it will return true if 
 * it was successful and if the directory entry is not null
*/
bool	globbing_loop(t_darray *files, const char *pat, t_globbing gl)
{
	bool	result;

	result = gl.dir_entry && !errno;
	while (result && gl.dir_entry)
	{
		gl.full_path = ft_strjoin3(gl.cwd, "/", gl.dir_entry->d_name);
		if (!gl.full_path)
			result = false_and_print("strjoin3");
		if (stat(gl.full_path, &gl.path_stat) == 0)
		{
			if (S_ISREG(gl.path_stat.st_mode))
			{
				if (is_glob_match(pat, gl.dir_entry->d_name))
					darray_push(files, ft_strdup(gl.dir_entry->d_name));
			}
		}
		else
			result = false_and_perr("stat");
		free(gl.full_path);
		errno = 0;
		gl.dir_entry = readdir(gl.dirp);
		if (!gl.dir_entry && errno)
			result = false_and_perr("readdir");
	}
	return (result && files->end > 0);
}

t_list	*create_globbing_tokenlist(t_darray *files)
{
	int		i;
	int		start;
	t_list	*head;
	t_list	*new_node;
	char	*file;

	i = 0;
	start = 0;
	head = NULL;
	new_node = NULL;
	while (i < files->end)
	{
		file = darray_get(files, i);
		new_node = new_toknode(WORD, file, &start, true);
		ft_lstadd_back(&head, new_node);
		i++;
	}
	return (head);
}

/*
 * This function will expand the globbing pattern
 * it will return nothing but will modify the tokenlist
 * if the pattern matches the files in the directory
*/
void	expand_globbing(t_list **tokenlist)
{
	char		*pat;
	t_list		*head;
	t_darray	*files;

	head = NULL;
	pat = get_token_lexeme(*tokenlist);
	debug("Expanding globbing pattern: %s", pat);
	files = darray_create(sizeof(char *), 100);
	if (match_files_in_directory(files, pat))
		replace_token_with_tokenlist(&head,  tokenlist, create_globbing_tokenlist(files));
	if (head)
		*tokenlist = head;
	debug("Expanded globbing pattern: %s first lexeme %s", pat, get_token_lexeme(*tokenlist));
	print_tokenlist(*tokenlist);
	darray_clear_destroy(files);
	return ;
}
