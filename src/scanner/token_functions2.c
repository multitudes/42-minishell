/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:24:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/11 20:38:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
sometimes you need to know where you head is at
*/
t_list	*get_head(t_list *tokenlist)
{
	t_list	*head;

	head = tokenlist;
	while (head->prev)
		head = head->prev;
	return (head);
}

int	count_tokens(t_list *tokenlist)
{
	int	count;

	count = 0;
	while (tokenlist) 
	{
		count++;
		tokenlist = tokenlist->next;
	}
	return (count);
}
