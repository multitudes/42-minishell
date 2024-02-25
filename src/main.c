/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:02:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/25 13:20:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <readline/readline.h>
#include <readline/history.h>

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

	// readline example
	char *input;
    input = readline("Enter some text: ");
    if (input != NULL) {
        printf("You entered: %s\n", input);
        free(input);
    }

	// check if interactive or not example
	if (isatty(STDIN_FILENO))
	{
		ft_printf("Interactive mode\n");
	}
	else
	{
		ft_printf("Non-interactive mode\n");
	}
		
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

/*
example testing
true && echo "This command always succeeds"
false || echo "This command always fails"

BUILTINS
the -n option prevents echo from adding a newline character
$ echo -n "Hello, "; echo "World "
Hello, World

cd with only a relative or absolute path - pwd with no options
$ pwd
/home/user/Documents
$ cd Downloads
$ pwd
/home/user/Documents/Downloads
$ pwd
/home/user/Documents
$ cd /var/log
$ pwd
/var/log

export with no options
$ export MY_VARIABLE="Hello, World!"
$ echo $MY_VARIABLE
Hello, World!

unset with no options - used to unset or remove variables or functions
$ MY_VARIABLE="Hello, World!"
$ echo $MY_VARIABLE
Hello, World!
$ unset MY_VARIABLE
$ echo $MY_VARIABLE

env with no options or arguments
prints the current environment variables. 
$ env
USER=john_doe
HOME=/home/john_doe
PATH=/usr/local/bin:/usr/bin:/bin

exit with no options
$ exit
#!/bin/bash
echo "This is a script."
exit

or > exit
exits minishell and returns to the terminal


...


*/