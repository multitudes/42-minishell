/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:11:00 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:42:51 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
 * again for overview. This is the data struct
 * living inside the scanner. It holds 
 * - the input
 * - error flag to stop the loop
 * - error string to print the error
 * - the token list to return to the parser or free
 * 
 * struct s_mini_data 
 * {
 * 	const char	*input;
 * 	t_list		*tokenlist;
 * 	bool		scanner_error;
 * 	char		*scanner_err_str;
 * };
 * typedef struct s_mini_data t_mini_data;
 */
int	init_scanner_data(t_mini_data *data, const char *input)
{
	data->input = input;
	data->tokenlist = NULL;
	data->scanner_error = false;
	data->scanner_err_str = NULL;
	return (1);
}
