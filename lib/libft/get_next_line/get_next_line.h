/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:02:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/04 14:26:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include "../libft.h"

typedef struct s_blk
{
	ssize_t			size;
	ssize_t			has_nl;
	void			*content;
	struct s_blk	*next;
}					t_blk;

char	*get_next_line(int fd);

char	*_get_line_and_stash(t_blk **line, char *buf, ssize_t n);
int		_fr(t_blk **line, char *s);
ssize_t	_app_str(t_blk *line, char *str, ssize_t n);
ssize_t	_strchr_newline(char *str);
t_blk	*_lstnew(void *content, ssize_t n);
int		_safety_check(int fd, char **buf, ssize_t *n);
ssize_t	read_again(int fd, char **buf, ssize_t *n);
ssize_t	_get_content_in_lst(t_blk *node, char *line_string);

#endif
