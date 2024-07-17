/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splash_error.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:23:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/15 12:53:44 by lbrusa           ###   ########.fr       */
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

void *perror_and_null(const char *msg);
bool	perror_and_bool(const char *msg, bool boolean);
uint8_t	perror_and_status(const char *msg, uint8_t status);
uint8_t	perror_and_status2(const char *msg_1, const char *msg_2, \
							uint8_t status);
uint8_t	perror_and_exit_with_status(const char *msg, uint8_t status);
uint8_t	stderr_and_status(const char *msg, uint8_t status);

uint8_t	stderr_and_status2(const char *msg_1, const char *msg_2, \
							uint8_t status);
uint8_t	stderr_and_status3(const char *msg_1, \
							const char *msg_2, const char *msg_3, \
							uint8_t status);
bool	stderr_and_bool(const char *msg, bool boolean);

#  ifdef __cplusplus

}
#  endif

# endif
