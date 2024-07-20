/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:52:04 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/19 12:36:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
void ft_lstdelone(t_list *lst, void (*del)(void*));

 lst:  The node to free.
 del:  The address of the function used to delete
 the content.
 Takes as a parameter a node and frees the memory of
 the node’s content using the function ’del’ given
 as a parameter and free the node.  The memory of
 ’next’ must not be freed.
 */
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}
