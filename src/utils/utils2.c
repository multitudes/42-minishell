/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:37:39 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/14 17:28:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "utils.h"

/*
util function to replace a token with a new 
tokenlist
*/
void	replace_token_with_tokenlist(t_list **head, t_list **tokenlist, t_list *new_tokenlist)
{
	t_list	*next;
	t_list	*prev;
	t_list	*last;
	t_list	*current_pos;

	current_pos = *tokenlist;
    if (!tokenlist || !*tokenlist || !new_tokenlist) 
		return;
	next = (*tokenlist)->next;
	prev = (*tokenlist)->prev;
	if (prev)
	{
		prev->next = new_tokenlist;
		new_tokenlist->prev = prev;
	}
	else
		*head = new_tokenlist;
	current_pos = new_tokenlist;
	last = ft_lstlast(new_tokenlist);
	last->next = next;
	if (next)
		next->prev = last;
	ft_lstdelone(*tokenlist, free_tokennode);
	*tokenlist = current_pos;
}

/*
 * This function will replace the node with the new list
 * it will return nothing but will modify the tokenlist
 * params: 
 * head - the head of the tokenlist - will be modified only if the newlist
 * is the first node
 * node - the node to be replaced - it will be freed also
 * newlist - the new list to replace the node
*/
void	replace_node_with_newlist(t_list **node, t_list *newlist)
{
	t_list *prev;
	t_list *next;
	t_list *last;
	t_list *tmp;
	
	if (!node || !newlist)
		return ;
	tmp = *node;
	prev = (*node)->prev;
	next = (*node)->next;
	if (prev)
		prev->next = newlist;
	else
		tmp = newlist;
	newlist->prev = prev;
	last = ft_lstlast(newlist);
	last->next = next;
	if (next)
		next->prev = last;
	ft_lstdelone(*node, free_tokennode);
	if (!prev)
		*node = tmp;
}

/*
wrapper for the system write function to simplify syntax
and perform write return value checking.
Returns FALSE if write call returns error.
Returns TRUE if empty string is passed as argument so that !
*/
bool	ft_write(int fd, const char *str)
{
	ssize_t	write_return;

	write_return = 0;
	if (!str)
		return (true);
	write_return = write(fd, str, ft_strlen(str));
	if (write_return == -1 || write_return != (ssize_t)ft_strlen(str))
	{
		perror("minishell: write");
		return (false);
	}
	return (true);
}

/*
Write to fd and update status in case of write failure.
*/
bool	write_data(int fd, const void *str, uint8_t *status)
{
	if (!ft_write(fd, str))
	{
		*status = EXIT_FAILURE;
		return (false);
	}
	return (true);
}