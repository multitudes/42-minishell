/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splash_error.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 17:34:48 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLASH_ERROR_H
# define SPLASH_ERROR_H

# include <stdint.h>
# include <stdbool.h>

// needed for the tests - leave it here - norminette allows it I think 
# ifdef __cplusplus

extern "C" {
# endif

void	perror_minishell(const char *msg);
bool	perror_and_bool(const char *msg, bool return_value);
uint8_t perror_and_status(const char *msg, uint8_t status);
uint8_t	perror_and_status2(const char *msg_1, const char *msg_2, uint8_t status);

uint8_t	perror_and_exit_with_status(const char *msg, uint8_t status);
uint8_t	print_error_status(const char *message, uint8_t status);
uint8_t	print_minishell_error_status(const char *message, uint8_t status);
uint8_t	print_error_status2(const char *message, const char *message2, \
							uint8_t status);
uint8_t	zero_and_printerr(const char *msg);
void	*null_on_err(const char *message);
bool	false_and_print(const char *msg);


#  ifdef __cplusplus

}
#  endif

# endif
