/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 12:05:35 by lbrusa           ###   ########.fr       */
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

bool init(t_globbing *gl)
{
	gl->dirp = opendir(".");
	if (!gl->dirp)
		return (false_and_perr("opendir"));
	gl->dir_entry = readdir(gl->dirp);
	gl->full_path = NULL;
	errno = 0;
	if (getcwd(gl->cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		if (closedir(gl->dirp))
			perror("closedir");
		return (false);
	}
	gl->full_path = NULL;
	return (true);
}

bool 	globbing_loop(t_darray *files, const char *pat, t_globbing gl)
{
	while (gl.dir_entry && !errno)
	{
		gl.full_path = ft_strjoin3(gl.cwd, "/", gl.dir_entry->d_name);
		if (!gl.full_path)
		{
			if (closedir(gl.dirp))
				perror("closedir");
			return (false);
		}
		if (stat(gl.full_path, &gl.path_stat) == 0)
		{
			if (S_ISREG(gl.path_stat.st_mode))
			{
				if (is_glob_match(pat, gl.dir_entry->d_name))
					darray_push(files, ft_strdup(gl.dir_entry->d_name));
			}
		}
		else
			perror("stat");
		free(gl.full_path);
		errno = 0;
		gl.dir_entry = readdir(gl.dirp);
	}
	if (errno)
		perror("readdir");
}

/*
 * I need to read the files in my directory and check if they match the pattern
 * this function will push a list of files in my darray and return true if it 
 * was successful
*/
bool	match_files_in_directory(t_darray *files, const char *pat) 
{
	t_globbing gl;

	if (!init(&gl))
		return (false_and_perr("globbing init"));
	if (!globbing_loop(files, pat, gl))
		return (false);
	if (closedir(gl.dirp))
		false_and_perr("closedir");
	return (true);
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
		{
			return 1;
		}
        return (*file_name != '\0' && is_glob_match(pat, file_name + 1));
    }
    return (*pat == *file_name && is_glob_match(pat + 1, file_name + 1));
}


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
		int i = 0;
		int start = 0;
		while (i < files->end)
		{
			char *file = darray_get(files, i);
			debug("file: %s", file);
			t_list *new_node = new_toknode(WORD, file, &start);
			debug("new node: %s", get_token_lexeme(new_node));
			ft_lstadd_back(&head, new_node);
			((t_token*)new_node->content)->type = WORD;
			i++;
		}
		debug("head still: %s and next %s", get_token_lexeme(head), get_token_lexeme(head->next));
		t_list *last = ft_lstlast(head);
		last->next = next;
		debug("last still: %s and next %s", get_token_lexeme(last), get_token_lexeme(next));
		if (next)
			next->prev = last;
		// free the old list
		debug("tokenlist %s and next %s", get_token_lexeme(tokenlist), get_token_lexeme(tokenlist));

		ft_lstdelone(tokenlist, free_tokennode);
		// free the darray
	}
	else 
	    return ;
	darray_clear_destroy(files);
	return ;
}