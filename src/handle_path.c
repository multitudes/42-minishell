/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 17:31:01 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/30 18:42:10 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "handle_path.h"



/*
The PATH env variable need to be split into a linked list of paths.
*/
t_list *get_path_linked_list(char *path)
{
	t_list *path_list;
	t_list *tmp;
	char *ptr;

	if (path == NULL)
		return (NULL);
	path_list = NULL;
	tmp = NULL;
	while (*path)
	{
		tmp = malloc(sizeof(t_list));
		if (tmp == NULL)
		{
			ft_lstclear(&path_list, free);
			return (NULL);
		}
		ptr = ft_strchr(path, ':');

		if (ptr == NULL)
			tmp->content = (void *)ft_strdup(path);
		else
			tmp->content = (void *)ft_substr(path, 0, ptr - path);
		// debug("path: %s", (char *)tmp->content);
		if (tmp->content == NULL)
		{
			ft_lstclear(&path_list, free);
			free(tmp);
			return (NULL);
		}
		ft_lstadd_back(&path_list, tmp);
		if (ptr == NULL)
			break;
		path = ptr;
		path++;
	}
	return (path_list);
}

/*
 print the linked list of paths
*/
void print_path_list(t_list *path_list)
{
	t_list *tmp;

	tmp = path_list;
	while (tmp)
	{
		printf("path: %s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}