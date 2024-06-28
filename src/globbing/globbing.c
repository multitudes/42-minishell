/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/02 17:41:39 by lbrusa           ###   ########.fr       */
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
#include "error.h"
#include "parser.h"

/*
 * This function will initialize the struct used in globbing func
 * it will return true if it was successful
*/
bool init(t_globbing *gl)
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
	{
		return (false_and_perr("getcwd"));
	}
	gl->full_path = NULL;
	return (true);
}

/*
 * This function will loop through the directory and check if the files match the pattern
 * it will return true if it was successful and if the directory entry is not null
*/
bool 	globbing_loop(t_darray *files, const char *pat, t_globbing gl)
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

/*
 * I need to read the files in my directory and check if they match the pattern
 * this function will push a list of files in my darray and return true if it 
 * was successful
*/
bool	match_files_in_directory(t_darray *files, const char *pat) 
{
	t_globbing gl;
	bool	result;

	result = true;
	if (!init(&gl))
		result = false;
	if (result == false || !globbing_loop(files, pat, gl))
		result = false;
	if (closedir(gl.dirp))
		false_and_perr("closedir");
	return (result);
}

/*
 * This recursive function will check if the pattern matches the file name
 * it will return true if it does
*/
bool	is_glob_match(const char *pat, const char *file_name)
{
    if (*pat == '\0')
        return (*file_name == '\0');
    if (*pat == '?')
        return (*file_name != '\0' && is_glob_match(pat + 1, file_name + 1));
    if (*pat == '*') 
	{
        if (is_glob_match(pat + 1, file_name))
			return 1;
        return (*file_name != '\0' && is_glob_match(pat, file_name + 1));
    }
    return (*pat == *file_name && is_glob_match(pat + 1, file_name + 1));
}

t_list *create_globbing_tokenlist(t_darray *files)
{
	t_list *head = NULL;
	int i = 0;
	int start = 0;
	while (i < files->end)
	{
		char *file = darray_get(files, i);
		t_list *new_node = new_toknode(WORD, file, &start);
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
void	expand_globbing(t_list *tokenlist)
{
	char		*pat;
	t_darray	*files;
	
	pat = get_token_lexeme(tokenlist);
	files = darray_create(sizeof(char *), 100);
	if (match_files_in_directory(files, pat))
	{
		t_list *next = tokenlist->next;
		t_list *head = tokenlist->prev;
		head->next = NULL;
		tokenlist->next = NULL;
		tokenlist->prev = NULL;
		
		head->next = create_globbing_tokenlist(files);
		
		t_list *last = ft_lstlast(head);
		last->next = next;
		if (next)
			next->prev = last;
		ft_lstdelone(tokenlist, free_tokennode);
	}
	darray_clear_destroy(files);
	return ;
}