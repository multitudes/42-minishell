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

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "libft.h"
# include "scanner.h"
# include "environment.h"
# include "history.h"
# include "utils.h"
# include <unistd.h>

// needed for the tests - leave it here
# ifdef __cplusplus

extern "C" {

# endif

uint8_t execute_builtin(t_list * tokenlist, t_data * data);
uint8_t		execute_cd_builtin(t_darray *env_arr, t_list *tokenlist);
uint8_t		execute_cd_tokenlist(t_darray *env_arr, t_list *tokenlist);
char		*execute_getcwd(char old_dir[]); //, char *message);
uint8_t		execute_echo_builtin(t_list *tokenlist);
uint8_t		execute_env_builtin(t_darray *env_arr, t_list *tokenlist);
uint8_t		execute_exit_builtin(t_data *data, t_list *tokenlist);
uint8_t		execute_export_builtin(t_darray *env_arr, t_list *tokenlist);
uint8_t		execute_pwd_builtin(void);
uint8_t		execute_unset_builtin(t_darray *env_arr, t_list *tokenlist);
int			merge_tokens(t_list *tokenlist);
bool		allowed_flags(const char *flag_lexem, const char *allowed_flags);
bool		ft_isnumstring(const char *str);
bool		read_only_variable(const char *key);
bool		no_valid_identifier(const char *key);

#  ifdef __cplusplus

}
#  endif

# endif
