/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:20:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/02/25 13:21:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
BUILTINS to implement:
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


*/