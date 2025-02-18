/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 09:40:56 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 13:40:01 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"

int	main(int argc, char **argv)
{
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
		return (single_command_loop(argv[2]));
	else
		return (loop());
}
