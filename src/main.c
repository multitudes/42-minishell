/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:02:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/19 18:21:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <readline/readline.h>
#include <readline/history.h>
int	main(void)
{
	// char input[1024];
	ft_printf("Hello, World!\n");

char *input;

    input = readline("Enter some text: ");

    if (input != NULL) {
        printf("You entered: %s\n", input);
        free(input); // Important to free the memory allocated by readline
    }


	// while (1)
	// {
	// 	ft_printf("$ ");
	// 	// gett user input
	// 	// fork the child process
		
		
	// }

	return (0);
}
