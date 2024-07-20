/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 17:41:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/16 12:01:40 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
SYNOPSIS
void ft_lstadd_front(t_list **lst, t_list *new);

 lst:  The address of a pointer to the first link of
 a list.
 new:  The address of a pointer to the node to be
 added to the list.
 Adds the node ’new’ at the beginning of the list.
 node->next = list->first;
 list->first->prev = node;
 list->first = node;
 */
void	ft_lstadd_front(t_list **lst, t_list *new_node)
{
	if (new_node != NULL && *lst != NULL)
	{
		(*lst)->prev = new_node;
		new_node->next = *lst;
	}
	*lst = new_node;
}
