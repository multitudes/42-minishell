/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:38:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/28 17:08:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANALYSER_H
# define ANALYSER_H

# include "minishell.h"

void	analyse_expand(t_ast_node *ast, t_mini_data *data);
void	which_ast_node(t_ast_node *ast);

#endif  // ANALYSER_H_
