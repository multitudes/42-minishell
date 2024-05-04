/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:19:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/20 18:44:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "minishell.h"

int		execute_ast(t_ast_node *ast, t_mini_data *data);
void	execute_builtin(t_list *tokenlist, t_mini_data *data);
int		execute_command(t_list *tokenlist, t_mini_data *data);




#endif  // EXECUTER_H_
