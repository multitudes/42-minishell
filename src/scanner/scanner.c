/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/05/12 19:44:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
condition is a pointer to a function that will be used to check the
character in the wjile loop. I might need is_digit for numbers or
is_alnum for identifiers etc
Thgis works for easy tokens.
*/
bool	process_token(t_mini_data *data, int *i, bool (*cnd)(char), int type)
{
	int		start;
	char	*tmp;

	start = *i;
	while (cnd(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	add_token(data, &start, tmp, type);
	free(tmp);
	*i = start;
	return (true);
}

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
		return (scanner_error(data, "error: unclosed expression"));
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
	while (peek(data->input + *i, delim, false) == false)
		advance(i);
	if (*(data->input + *i + 1) == '\0')
		return (scanner_error(data, "error: unclosed expansion"));
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
History expansion - (not implemented)!
!!: Re-run the previous command. 
This is useful if you forgot to use sudo for a command that 
requires it. You can simply type sudo !! to re-run the previous 
command with sudo.

!n: Re-run the nth command in your history. 
For example, !100 would re-run the 100th command.

!-n: Re-run the command n lines back. 
For example, !-2 would re-run the second most recent command.

!string: Re-run the most recent command that 
starts with string. For example, !ls would re-run the most recent command 
that starts with ls.

!?string?: Re-run the most recent command 
that contains string anywhere. For example, !?txt? would re-run the most 
recent command that includes txt.
BANG_BANG,BANG_DIGIT, BANG_HYPHEN_DIGIT, BANG_ALPHA, BANG_QUESTION_ALPHA
*/
bool	is_a_hist_expansion(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "!!", false))
		return (process_token(data, i, is_not_delimiter, BANG_BANG));
	else if (peek(data->input + *i, "!", false) && is_digit(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_digit, BANG_DIGIT));
	else if (peek(data->input + *i, "!-", false) && is_digit(*(data->input \
	+ *i + 2)))
		return (proc_tok_off_2(data, i, is_digit, BANG_HYPHEN_DIGIT));
	else if (peek(data->input + *i, "!", false) && is_alpha(*(data->input \
	+ *i + 1)))
		return (proc_token_off_1(data, i, is_alpha, BANG_ALPHA));
	else if (peek(data->input + *i, "!?", false))
		return (process_token(data, i, is_not_delimiter, BANG_QUESTION_ALPHA));
	else if (peek(data->input + *i, "!#", false))
		return (add_token(data, i, "!#", BANG_HASH));
	return (false);
}


bool	add_here_and_delim(t_mini_data *data, int *i)
{
	int		start;
	char	*tmp;

	add_token(data, i, "<<", DLESS);
	while (data->input[*i] && is_space(data->input[*i]))
		advance(i);
	if (data->input[*i] == '\0' || is_delimiter(data->input[*i]))
		return (scanner_error(data, "error: missing here-delim"));
	start = *i;
	while ((data->input + *i) && !is_delimiter(data->input[*i]))
		advance(i);
	tmp = ft_substr(data->input, start, *i - start);
	*i = start;
	add_token(data, i, tmp, DLESS_DELIM);
	free(tmp);
	return (true);
}

bool	is_a_simple_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, ">|", false))
		return (add_token(data, i, ">|", CLOBBER));
	else if (peek(data->input + *i, ">>&", false))
		return (add_token(data, i, ">>&", REDIRECT_BOTH_APP));
	else if (peek(data->input + *i, "<<-", false))
		return (add_token(data, i, "<<-", DLESSDASH));
	else if (peek(data->input + *i, "&>>", false))
		return (add_token(data, i, "&>>", REDIRECT_OUT_APP));
	else if (peek(data->input + *i, ">&>", false))
		return (add_token(data, i, ">&>", GREATER_AND_GREATER));
	else if (peek(data->input + *i, "&>", false))
		return (add_token(data, i, "&>", REDIRECT_BOTH));
	else if (peek(data->input + *i, "<&", false))
		return (add_token(data, i, "<&", LESSAND));
	else if (peek(data->input + *i, ">&", false))
		return (add_token(data, i, ">&", GREATAND));
	else if (peek(data->input + *i, "<>", false))
		return (add_token(data, i, "<>", LESSGREAT));
	else if (peek(data->input + *i, ">>", false))
		return (add_token(data, i, ">>", DGREAT));
	else
		return (false);
	return (true);
}

bool	is_a_aggregate_redirection(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "<<", false))
		return (add_here_and_delim(data, i));
	else if (peek(data->input + *i, ">=", false))
		return (add_token(data, i, ">=", GREATER_EQUAL));
	else if (peek(data->input + *i, "<=", false))
		return (add_token(data, i, "<=", LESS_EQUAL));
	else if (peek(data->input + *i, "!=", false))
		return (add_token(data, i, "!=", BANG_EQUAL));
	else if (peek(data->input + *i, ">", false))
		return (add_token(data, i, ">", REDIRECT_OUT));
	else if (peek(data->input + *i, "<", false))
		return (add_token(data, i, "<", REDIRECT_IN));
	else if (peek(data->input + *i, "==", false))
		return (add_token(data, i, "==", EQUAL_EQUAL));
	else if (peek(data->input + *i, ",", false))
		return (add_token(data, i, ",", COMMA));
	else if (peek(data->input + *i, "-", true))
		return (add_token(data, i, "-", MINUS));
	else if (peek(data->input + *i, "+", false))
		return (add_token(data, i, "+", PLUS));
	else if (peek(data->input + *i, "*", false))
		return (add_token(data, i, "*", STAR));
	else
		return (false);
}

bool	is_a_redirection(t_mini_data *data, int *i)
{
	if (is_a_simple_redirection(data, i))
		return (true);
	else if (is_a_aggregate_redirection(data, i))
		return (true);
	else
		return (false);
}

/*
*/
bool	is_a_block(t_mini_data *data, int *i)
{
	if (peek(data->input + *i, "(", false))
		return (add_tokenblock(data, i, ')', EXPRESSION));
	else if (peek(data->input + *i, "\'", false))
		return (add_tokenblock(data, i, '\'', S_QUOTED_STRING));
	else if (peek(data->input + *i, "\"", false))
		return (add_tokenblock(data, i, '\"', QUOTED_STRING));
	else if (peek(data->input + *i, "`", false))
		return (add_tokenblock(data, i, '`', COM_EXPANSION));
	return (false);
}

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
	while (is_delimiting_char(data->input[*i]))
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
t_list	*tokenizer(t_mini_data *data)
{
	int	i;

	i = 0;
	data->exit_status = 0;
	data->token_list = NULL;
	while (i < (int)ft_strlen(data->input) && data->exit_status == 0)
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
	if (data->exit_status == 0)
		return (data->token_list);
	ft_lstclear(&data->token_list, free_token);
	return (NULL);
}
