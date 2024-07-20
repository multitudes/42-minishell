/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:02:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/04 14:25:41 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
I had to add because valgrind complained about using a not initialised 
variable. It was not true because I was initializing my final string
in the next function but it has badly interpreted so I replaced my malloc
with a ft_calloc from the libft (modified)
*/
// void	*ft_calloc(size_t count, size_t size)
// {
// 	void	*p;
// 	char	*tmp;
// 	size_t	n;

// 	if (count == 0 || size == 0)
// 	{
// 		count = 1;
// 		size = 1;
// 	}
// 	if (count >= (SIZE_MAX / size))
// 		return (NULL);
// 	p = malloc(count * size);
// 	if (p == NULL)
// 		return (NULL);
// 	n = count * size;
// 	tmp = p;
// 	while (n > 0)
// 	{
// 		*tmp++ = 0;
// 		n--;
// 	}
// 	return (p);
// }

/*
 SYNOPSIS
 Allocates (with malloc(3)) and returns a new node.
 The member variable ’content’ is initialized with
 the value of the parameter ’content’.  The variable
 ’next’ is initialized to NULL.
 //	printf(" inside new === > %s\n",((char*)new->content));
 */
t_blk	*_lstnew(void *content, ssize_t n)
{
	t_blk	*new;

	new = malloc(sizeof(t_blk));
	if (new == NULL)
		return (NULL);
	new->content = malloc(sizeof(void *) * (n + 1));
	if (new->content == NULL)
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	new->size = n;
	((char *)new->content)[n] = 0;
	while (n-- > 0)
		((char *)new->content)[n] = ((char *)(content))[n];
	new->has_nl = _strchr_newline((char *) new->content);
	return (new);
}

/*
similar to strchr but specialized for a newline char
*/
ssize_t	_strchr_newline(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

/*
 app is for append! I had to shorten my line for norm!
 */
ssize_t	_app(t_blk *line, char *str, ssize_t n)
{
	t_blk	*new;

	if (line == NULL)
		return (0);
	new = _lstnew(str, n);
	if (new == NULL)
		return (0);
	line->size += new->size;
	line->has_nl = _strchr_newline(str);
	while (line->next != NULL)
		line = line->next;
	line->next = new;
	return (1);
}

/*
 Function name get_next_line
 
 Prototype  char *get_next_line(int fd);
 Parameters fd:  The file descriptor to read from
 External functs. read, malloc, free
 Turn in files
 get_next_line.c, get_next_line_utils.c,
 get_next_line.h
 Return value Read line:  correct behavior
 NULL: there is nothing else to read, or an error
 occurred

 Description:
 Write a function that returns a line read from a
 file descriptor
 params:
 fd: file descriptor to read from
 return: a string read from fd without the newline
 
 line is a linked list collecting the strings ready to be
 outputted as a string when collated and a newline has been found
 initialized as a t_blk which 
 t_blk contains the total size of chars in the first link
 and also if the chain has a new line. I store this info so i 
 do not need to walk through the chain again.
 Used as static var automatically inits its contents to null.
 */
char	*get_next_line(int fd)
{
	ssize_t			n;
	char			*buf;
	static t_blk	*line[OPEN_MAX];

	if (!_safety_check(fd, &buf, &n))
		return (NULL);
	if (line[fd] == NULL || line[fd]->has_nl < 0)
	{
		n = read(fd, buf, BUFFER_SIZE);
		while (n > 0)
		{
			buf[n] = 0;
			if (line[fd] == NULL)
			{
				line[fd] = _lstnew(buf, n);
				if (line[fd] == NULL && _fr(&line[fd], buf))
					return (NULL);
			}
			else if (line[fd] && !_app(line[fd], buf, n) && _fr(&line[fd], buf))
				return (NULL);
			if (line[fd]->has_nl >= 0 || read_again(fd, &buf, &n) <= 0)
				break ;
		}
	}
	return (_get_line_and_stash(&line[fd], buf, n));
}
