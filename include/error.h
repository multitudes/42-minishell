/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/28 11:49:21 by lbrusa           ###   ########.fr       */
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

int error_set_status(char *msg, t_data *data);
int	exit_err_failure(char *msg) NORETURN;

#  ifdef __cplusplus

}
#  endif

# endif
