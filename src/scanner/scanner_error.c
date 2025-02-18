/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:07:56 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 12:51:07 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "splash.h"
#include "utils.h"
#include "splash_error.h"

int	init_scanner_data(t_mini_data *data, const char *input)
{
	data->input = input;
	data->tokenlist = NULL;
	data->scanner_error = false;
	data->scanner_err_str = NULL;
	return (1);
}

bool	scanner_error(t_mini_data *data, char *err_str)
{
	stderr_and_status(err_str, 0);
	data->scanner_err_str = err_str;
	data->scanner_error = 2;
	return (true);
}

void	free_tokennode(void *content)
{
	if (content)
		free(((t_token *)content)->lexeme);
	free(content);
}
