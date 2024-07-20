/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:41:25 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/16 11:54:33 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS=
void ft_lstadd_back(t_list **lst, t_list *new);
 
 lst:  The address of a pointer to the first link of
 a list.
 new:  The address of a pointer to the node to be
 added to the list.
 adds the node ’new’ at the end of the list.
 */
void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	last = ft_lstlast(*lst);
	if (last)
	{
		last->next = new_node;
		new_node->prev = last;
	}
	else
		*lst = new_node;
}
