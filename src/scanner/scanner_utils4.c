/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:03:58 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/12 15:12:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "debug.h"

bool	not_a_delimiting_char(const char c)
{
	return (ft_isprint(c) && !filename_delimiter(c));
}

/*
 * is_true_false checks if the identifier is a boolean value
 * like in bash true and false are not builtins but reserved words
 */
bool	is_true_false(t_mini_data *data, char *str, int *start)
{
	if (peek(str, "true", EXACT))
		add_token(data, start, "true", TRUETOK);
	else if (peek(str, "false", EXACT))
		add_token(data, start, "false", FALSETOK);
	else
		return (false);
	return (true);
}

void	print_token_list(t_list *token_list)
{
	t_list	*current;
	t_token	*token;

	debug("print tokenlist");
	current = token_list;
	while (current != NULL)
	{
		token = (t_token *)(current)->content;
		printf("token: %s", token->lexeme);
		current = current->next;
	}
	return ;
}

/*
io numbers are the one preceding a < or > in a redirection
cannot start with a 0
*/
bool	is_io_number(const char *str)
{
	if (*str == '0' && is_digit(*(str + 1)))
		return (false);
	while (is_digit(*str))
		str++;
	if (*str == '\0')
		return (true);
	return (false);
}

/*
including numbers preceded by a - or + 
and with and without a dot
*/
bool	str_is_number(const char *str)
{
	int	dot_seen;

	dot_seen = 0;
	if ((*str == '-' || *str == '+') && is_digit(*(str + 1)))
		str++;
	while (*str)
	{
		if (*str == '.')
		{
			if (dot_seen)
				return (false);
			dot_seen = 1;
		}
		else if (!is_digit(*str))
			return (false);
		str++;
	}
	return (true);
}
