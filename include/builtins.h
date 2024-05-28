/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 21:56:54 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/26 21:59:55 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

# include "minishell.h"
# include "libft.h"
# include "scanner.h"
# include "environment.h"
# include "history.h"
# include <unistd.h>

# define BUFREADSIZE 300

int	execute_builtin(t_list *tokenlist, t_data *data);
int	execute_cd_builtin(t_data *data, t_list *tokenlist);
int	execute_echo_builtin(t_list *tokenlist);
int	execute_env_builtin(t_data *data);
int	execute_pwd_builtin(void);

#  ifdef __cplusplus

}
#  endif

#endif
