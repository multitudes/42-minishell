/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:38:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 19:19:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <libft.h>

int     ft_isascii(const int c);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin3(const char *key, const char *mid, const char *value);
int	    count_char_in_str(char *str, char c);
void	replace_node_with_newlist(t_list **node, t_list *newlist);
int     ft_strcmp(const char *s1, const char *s2);

#endif  // UTILS_H_
