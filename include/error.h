/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/06 12:13:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

int		status_and_perror(char *msg, int status);
int     status_and_strjoin_perror(char *msg_1, char *msg_2, int status);
int		exit_and_print_err(char *msg, int status);
int     print_error_status(char *message, int status);
int	    print_minishell_error_status(char *message, int status);
int		zero_and_printerr(char *msg);
void	*null_on_err(char *message);
bool	false_and_perr(char *msg);
bool	false_and_print(char *msg);

#  ifdef __cplusplus

}
#  endif

# endif
