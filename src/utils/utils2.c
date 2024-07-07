/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:37:39 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/07 16:11:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "utils.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	if (!s1 && s2)
		return ((unsigned char)s2[0]);
	if (s1 && !s2)
		return ((unsigned char)s1[0]);
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}


/*
util function to replace a token with a new 
tokenlist
*/
void	replace_token_with_tokenlist(t_list **tokenlist, t_list *new_tokenlist)
{
	t_list	*next;
	t_list	*head;
	t_list	*last;
	t_list *current_pos;

	current_pos = *tokenlist;
    if (!tokenlist || !*tokenlist || !new_tokenlist) 
		return;
	next = (*tokenlist)->next;
	head = (*tokenlist)->prev;
	if (head)
	{
		head->next = new_tokenlist;
		new_tokenlist->prev = head;
	}
	else
		head = new_tokenlist;
	current_pos = new_tokenlist;
	last = ft_lstlast(new_tokenlist);
	last->next = next;
	if (next)
		next->prev = last;
	ft_lstdelone(*tokenlist, free_tokennode);
	*tokenlist = current_pos;
}
