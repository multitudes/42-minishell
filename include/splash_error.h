/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splash_error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/28 19:38:04 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

#include <stdint.h>
#include <stdbool.h>

uint8_t	status_and_perror(const char *msg, uint8_t status);
uint8_t	status_perror2(const char *msg_1, const char *msg_2, uint8_t status);
uint8_t	exit_and_print_err(const char *msg, uint8_t status);
uint8_t	print_error_status(const char *message, uint8_t status);
uint8_t	print_minishell_error_status(const char *message, uint8_t status);
uint8_t	zero_and_printerr(const char *msg);
void	*null_on_err(const char *message);
bool	false_and_perr(const char *msg);
bool	false_and_print(const char *msg);

#  ifdef __cplusplus

}
#  endif

# endif
