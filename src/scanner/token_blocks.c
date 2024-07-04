/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_blocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:51:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/12 15:08:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

/*
I define a block as the text between two delimiters like {}
or "" '' or `` or () etc
anything until I get the closing delimiter I specify in delim...
*/
bool	add_tokenblock(t_mini_data *data, int *i, char delim, int t_type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (data->input[*i] && data->input[*i] != delim)
		advance(i);
	if (data->input[*i] == '\0')
		return (scanner_error(data, "minishell: syntax error: unexpected end of file"));
	tmp = ft_substr(data->input, start, *i - start + 1);
	add_token(data, &start, tmp, t_type);
	*i = start;
	free(tmp);
	return (true);
}

/*
arithmetic expansion is $(()) and I need to find the closing ))
*/
bool	add_block_dbl_paren(t_mini_data *data, int *i, char *delim, int t_type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (peek(data->input + *i, delim, FUZZY) == false)
		advance(i);
	if (*(data->input + *i + 1) == '\0')
		return (scanner_error(data, "minishell: error: unclosed expansion"));
	tmp = ft_substr(data->input, start, *i - start + 2);
	*i = start;
	add_token(data, i, tmp, t_type);
	free(tmp);
	return (true);
}

/*
condition is a pointer to a function that will be used to check the
character in the wjile loop. I might need is_digit for numbers or
is_alnum for identifiers etc
Thgis works for easy tokens.
*/
bool	proc_token_off_1(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = (*i)++;
	while (cnd(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

/*
condition is a pointer to a function that will be used to check the
character in the while loop. I might need is_digit for numbers or
is_alnum for identifiers etc
This is offset of 2 lets say I look for !- followed by a number of digits
*/
bool	proc_tok_off_2(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = (*i);
	while (cnd(data->input[*i] + 2))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start + 3);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

/*
*/
bool	is_a_block(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "(", FUZZY))
		return (add_parenthesisblock(data, i, ')', EXPRESSION));
	else if (peek(data->input + *i, "\'", FUZZY))
		return (add_tokenblock(data, i, '\'', S_QUOTED_STRING));
	else if (peek(data->input + *i, "\"", FUZZY))
		return (add_tokenblock(data, i, '\"', QUOTED_STRING));
	else if (peek(data->input + *i, "`", FUZZY))
		return (add_tokenblock(data, i, '`', COM_EXPANSION));
	return (false);
}
