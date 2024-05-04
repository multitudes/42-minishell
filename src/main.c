/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:02:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/18 16:45:03 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
main is first of all a loop that runs the shell taking inputs from the user
and executing them until the user decides to exit it.
*/
int	main(int argc, char **argv)
{
	// provisional. But since we cannot test our main.c file we will continue
	// from here straight into a loop function that will be implemented in
	// loop.c file.
	// debug("argc: %d", argc);
	return (loop(argc, argv));
}
