/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:02:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/01/31 11:52:34 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	_get_content_in_lst(t_blk *node, char *line_string)
{
	ssize_t	i;
	ssize_t	pos;
	char	*content;

	i = 0;
	pos = -1;
	content = NULL;
	while (node != NULL)
	{
		content = (char *)node->content;
		while (*content != 0)
		{
			line_string[i++] = *content;
			if (pos == -1 && *content == '\n')
				pos = i ;
			content++;
		}
		node = node->next;
	}
	return (pos);
}

/*
 //	printf("(*line)->size %zd\n", (*line)->size);
 //		printf("node->content %s\n", (char *)node->content);
 //	printf("POS!!! %zd\n", pos);
 //		printf("line string is |%s| \n", line_string);
 //		printf("node string is |%s|\n", (char*)node->content);
 */
char	*_get_line_and_stash(t_blk **line, char *buf, ssize_t n)
{
	ssize_t	pos;
	char	*line_string;
	t_blk	*node;

	free(buf);
	if ((n <= -1 && _fr(line, NULL)) || (*line == NULL))
		return (NULL);
	line_string = ft_calloc(sizeof(char), ((*line)->size + 1));
	if (line_string == NULL || (*line)->size == 0)
		return (NULL);
	pos = _get_content_in_lst(*line, line_string);
	if (pos > 0 && ((*line)->size - pos > 0))
	{
		node = _lstnew(line_string + pos, (*line)->size - pos);
		line_string[pos] = 0;
		_fr(line, NULL);
		if (!node)
			return (NULL);
		node->has_nl = _strchr_newline((char *)node->content);
		*line = node;
	}
	else
		_fr(line, NULL);
	return (line_string);
}

int	_fr(t_blk **line, char *s)
{
	t_blk	**l;
	t_blk	*tmp;

	if (*line == NULL || line == NULL)
	{
		free(s);
		return (1);
	}
	l = line;
	tmp = *line;
	while ((*line)->next != NULL)
	{
		*line = (*line)->next;
		free(tmp->content);
		free(tmp);
		tmp = *line;
	}
	free(tmp->content);
	free(tmp);
	*l = NULL;
	free(s);
	return (1);
}

int	_safety_check(int fd, char **buf, ssize_t *n)
{
	*n = 1;
	if (fd >= OPEN_MAX)
		return (0);
	*buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!*buf)
		return (0);
	return (1);
}

ssize_t	read_again(int fd, char **buf, ssize_t *n)
{
	*n = read(fd, *buf, BUFFER_SIZE);
	return (*n);
}
