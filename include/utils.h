/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:38:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 17:03:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <libft.h>

int		ft_isascii(const int c);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
int		count_char_in_str(char *str, char c);
void	replace_node_with_newlist(t_list **node, t_list *newlist);
void	replace_token_with_tokenlist(t_list **tokenlist, t_list *new_tokenlist);

#endif  // UTILS_H_
