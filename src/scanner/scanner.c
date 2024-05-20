/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/20 15:09:53 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
create a lexeme for flag in this conf -[a-zA-Z]
else if (peek((data->input + i), "-", false) && is_alpha(data->input[i + 1]))
*/
bool	is_a_flag(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	if (peek(data->input + *i, "-", false) && is_alpha(data->input[*i + 1]))
	{
		start = (*i)++;
		while (data->input[*i] && is_alpha(data->input[*i]))
			advance(i);
		tmp = ft_substr(data->input, start, *i - start);
		add_token(data, &start, tmp, FLAGS);
		*i = start;
		free(tmp);
		return (true);
	}
	return (false);
}

bool	is_some_semicolons(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ";;&", false))
		return (add_token(data, i, ";;&", DSEMI_AND));
	else if (peek(data->input + *i, ";;", false))
		return (add_token(data, i, ";;", DSEMI));
	else if (peek(data->input + *i, ";", false))
		return (add_token(data, i, ";", SEMI));
	else if (peek(data->input + *i, "!", true))
		return (add_token(data, i, "!", BANG));
	else
		return (false);
}

bool	is_a_string_thing(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	start = *i;
	while (not_a_delimiting_char(data->input[*i]))
		advance(i);
	if (*i == start)
		return (false);
	tmp = ft_substr(data->input, start, *i - start);
	if ((data->input[*i] == '<' || data->input[*i] == '>') \
	&& is_io_number(tmp))
		add_token(data, &start, tmp, IO_NUMBER);
	else if (!is_reserved(data, tmp, &start) && !is_builtin(data, \
	tmp, &start) && !is_true_false(data, tmp, &start) && \
	!is_a_pathname_or_num(data, tmp, &start))
		add_token(data, &start, tmp, WORD);
	free(tmp);
	return (true);
}

/*

*/
bool	got_tokens(t_mini_data *data, int *i)
{
	if (is_a_control_operator(data, i) || is_a_math_op(data, i) || \
	is_a_hist_expansion(data, i) || is_a_dollar_exp(data, i) || \
	is_some_semicolons(data, i) || is_a_redirection(data, i) || \
	is_a_redirection(data, i) || is_simple_operator(data, i) || \
	is_a_block(data, i) || is_a_flag(data, i) || is_a_string_thing(data, i))
		return (true);
	else
		return (false);
}

/*
scanning function
Returns a token list which is a linked list of t_list type
note: we dont need to action some of them but fir completeness I added 
as much as I could. for instance we do not action the last & on a command
or redirections like 
*/
t_list	*tokenizer(const char *input)
{
	int			i;
	t_mini_data	*data;

	i = 0;
	data = NULL;
	if (!init_scanner_data(&data, input))
		return (NULL);
	while (i < (int)ft_strlen(data->input) && data->scanner_error == 0)
	{
		if (peek(data->input + i, "#", false))
			break ;
		else if (got_tokens(data, &i))
			continue ;
		else if (!is_space(data->input[i]))
			scanner_error(data, "error: unrecognized token");
		else
			i++;
	}
	if (data->scanner_error == 0)
		return (data->token_list);
	free_scanner_data(data);
	return (NULL);
}
