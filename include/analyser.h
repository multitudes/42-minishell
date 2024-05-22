/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:38:03 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/22 16:07:05 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANALYSER_H
# define ANALYSER_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif
# include "minishell.h"

void	analyse_expand(t_ast_node *ast, t_data *data);
void	which_ast_node(t_ast_node *ast);

#  ifdef __cplusplus

}
#  endif

# endif