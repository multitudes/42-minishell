/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/02 15:31:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

# include "minishell.h"

#ifdef __GNUC__
# define NORETURN __attribute__ ((__noreturn__)) 
# else
# define NORETURN
# endif

int		status_and_print(char *msg, int status);
int		exit_and_print_err(char *msg, int status);
int		zero_and_printerr(char *msg);
void	*return_null_on_err(char *message);
#  ifdef __cplusplus

}
#  endif

# endif
