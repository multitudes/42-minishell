/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:02:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/04/05 13:49:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
main is first of all a loop that runs the shell taking inputs from the user 
and executing them until the user decides to exit it.
*/
int	main(void)
{
	/*
	TODO:
	init and implement signals. ctrl-c, ctrl-d, ctrl-z, ctrl-\
	create error handling - check if input is valid - Incorrect command -
		incorrect number of arguments - permission denied - system call error
	check if interactive or not - display prompt then
	use readline function to read input
	scan and tokenixe input - for a start, just echo the input
	parsing the tokens - different type for each keyword, operator, 
		bit of punctuation, and literal type.
	*/
	ft_printf("Hello, World!\n");

	// check if interactive or not example
	// the isatty() function returns 1 if the file descriptor is an open file,
	// 0 if it is not, and -1 if an error occurs.


	// readline example

	/*
	The readLine() function, reads a line of input from 
	the user on the command line and returns the result. 
	To kill an interactive command-line app, type Control-D. 
	Doing so signals an “end-of-file” condition to the program. 
	When that happens readLine() returns null , 
	so we check for that to exit the loop.
	*/	

	return (0);
}
