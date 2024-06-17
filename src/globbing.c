/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:50:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 09:21:36 by lbrusa           ###   ########.fr       */
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
 * I need to read the files in my directory and check if they match the pattern
 * this function will push a list of files in my darray and return true if it 
 * was successful
*/
bool	match_files_in_directory(t_darray *files, const char *pat) 
{
    DIR *dirp;
	char cwd[PATH_MAX];
    struct dirent *dir_entry;
	struct stat path_stat;
	char *full_path;

    dirp = opendir(".");
    if (!dirp)
		return (false_and_perr("opendir"));
	errno = 0;
	dir_entry = readdir(dirp);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		if (closedir(dirp))
			perror("closedir");
		return (false);
	}
	debug("current working directory: %s\n", cwd);
	while (dir_entry && !errno)
	{
		debug("number of files in directory: %d\n", files->end);
		debug("-%s-", dir_entry->d_name);
		full_path = ft_strjoin3(cwd, "/", dir_entry->d_name);
		if (!full_path)
			return (false);
		// free
		debug("full path: %s\n", full_path);

		
		if (stat(full_path, &path_stat) == 0)
		{
			if (S_ISREG(path_stat.st_mode))
			{
				// debug("file considered: %s -----------------> ", dir->d_name);
				if (is_glob_match(pat, dir_entry->d_name))
					darray_push(files, ft_strdup(dir_entry->d_name));
			}
		}
		else
			perror("stat");

		free(full_path);
		errno = 0;
		dir_entry = readdir(dirp);
	}
	if (errno)
		perror("readdir");
	if (closedir(dirp))
		false_and_perr("closedir");
	return (true);
}

bool	is_glob_match(const char *pat, const char *file_name)
{
	debug("MATCHING pattern: %s file_name: %s", pat, file_name);
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
	char	*pat;
	t_darray	*files;
	
	// debug("expand_globbing");
	debug("token type: %d lexeme: %s", get_token_type(tokenlist), get_token_lexeme(tokenlist));
	pat = get_token_lexeme(tokenlist);
	files = darray_create(sizeof(char *), 100);
	if (match_files_in_directory(files, pat))
	{
		debug("files count : %d", files->end);
		
		t_list *next = tokenlist->next;
		t_list *head = tokenlist->prev;
		head->next = NULL;
		tokenlist->next = NULL;
		tokenlist->prev = NULL;
		debug("head still: %s and next %s", get_token_lexeme(head), get_token_lexeme(next));
		// create a new linked list of tokens with the file names
		int i = 0;
		int start = 0;
		while (i < files->end)
		{
			char *file = darray_get(files, i);
			debug("file: %s", file);
			t_list *new_node = new_toknode(WORD, file, &start);
			debug("new node: %s", get_token_lexeme(new_node));
			ft_lstadd_back(&head, new_node);
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
		// ft_lstclear(&tokenlist, free_tokennode);
		// ft_lstdelone(tokenlist, free_tokennode);
		// free the darray
	}
		darray_clear_destroy(files);
	


	return ;
}