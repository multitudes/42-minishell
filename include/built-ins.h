/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 21:56:54 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/26 21:59:55 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

# include "minishell.h"

void	execute_builtin(t_list *tokenlist, t_data *data);

# ifdef __cplusplus

}
# endif

#endif
