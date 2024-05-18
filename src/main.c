/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 09:40:56 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/18 15:49:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
main is first of all a loop that runs the shell taking inputs from the user
and executing them until the user decides to exit it.
provisional. But since we cannot test our main.c file we will continue
from here straight into a loop function that will be implemented in
loop.c file.
debug("argc: %d", argc);
*/
int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	return (loop());
}
