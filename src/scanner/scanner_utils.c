/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:48:36 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 18:50:46 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
 ``\t''``\n''``\v''``\f''``\r''`` -> is space 
 overkill I think. From the bash manual they talk only tab and space and 
 newline which in my case it is impossible because caught by the readline func
*/
bool	is_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' \
	|| c == '\r');
}

/*
string n compare but case insensitive
*/
int	strncicmp(char const *a, char const *b, int n)
{
	int	d;

	if (n == 0)
		return (0);
	if (a == NULL || b == NULL || n < 0)
		return (-1);
	while (*a && *b && n--)
	{
		d = ft_tolower(*a) - ft_tolower(*b);
		if (d != 0)
			return (d);
		a++;
		b++;
	}
	return (*a - *b);
}

/* Returns 'true' if the character is a delimiter. Because an identifier
can be unofficially almost anything, the delimiter is a character that
should not be part of an identifier. Recommended char for identifiers
are underscore and alphanumeric characters.
but from the bash manual:
A metacharacter is a space, tab, newline, 
or one of the following characters: 
‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
I use this function to understand where to break the string
into tokens
*/
bool	is_delimiter(const char ch)
{
	if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '|' || \
	ch == '&' || ch == ';' || ch == '(' || ch == ')' || \
	ch == '>' || ch == '<' || ch == '\0')
		return (true);
	return (false);
}

/*
 ... touch w$orld is creating w because dereferencing 
the $ and the rest is nulll...
otherwise 
touch w-+*}\,[]=w{$HOME
becomes touch: w-+*},[]=w{/Users/laurentb: Not a directory
touch w-+*}\,[]=w{&orld
bash: orld: command not found
touch w-+*}\,[]=w{|orld
bash: orld: command not found
[1]+  Done                    touch w-+*}\,[]=w{
The command touch w-+*}\,[]=w{|orld is being interpreted by the 
shell (bash in this case) before it's passed to the touch command.
In this case, touch w-+*}\,[]=w{ is being 
run as one command, and orld is being interpreted as a 
separate command.
The output of touch w-+*}\,[]=w{ (which is nothing, 
because touch doesn't produce any output) is being passed as input to orld.
However, orld is not a valid command, 
so bash gives an error message: bash: orld: command not found.
The [1]+ Done touch w-+*}\,[]=w{ message is from bash's job 
control system. It's telling you that the touch w-+*}\,[]=w{ 
	command has finished running.


touch w-+*},[]=w{\o!rld bash: !rld: event not found

The ! character has a special meaning in many Unix and Linux shells, 
including bash. It is used for history expansion, 
allowing you to re-run previous commands.
In your command touch w-+*}\,[]=w{\o!rld, the ! character 
is causing bash to attempt to perform history expansion

ok these char cannot be in a file name.. 
officially it is recommended to use only the 65 characters [-._a-zA-Z0-9]
for files and directories ...
*/
bool	filename_delimiter(const char ch)
{
	if (is_space(ch) || ch == ';' || ch == '>' || \
	ch == '<' || ch == '(' || ch == ')' || \
	ch == '|' || ch == '&' || ch == '$' || ch == '`' || \
	ch == '"' || ch == '\'' || ch == '!' || ch == '\0')
		return (true);
	return (false);
}

/*
Returns 'true' if the character is a digit
*/
bool	is_digit(const char c)
{
	return (c >= '0' && c <= '9');
}