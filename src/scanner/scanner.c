/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 19:47:11 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/16 15:30:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "debug.h"
#include "utils.h"
#include "parser.h"

/*
 * scanning function
 * Returns a token list which is a linked list of t_list type
 * note: we dont need to action some of them but fir completeness I added 
 * as much as I could. for instance we do not action the last & on a command
 * or redirections like 
 */
t_list	*tokenizer(const char *input)
{
	int			i;
	t_mini_data	data;

	i = 0;
	if (!init_scanner_data(&data, input))
		return (NULL);
	while (i < (int)ft_strlen(data.input) && !data.scanner_error)
	{
		if (got_tokens(&data, &i))
			continue ;
		else if (!is_space(data.input[i]))
			scanner_error(&data, "minishell: syntax error");
		else
			skip_space(data.tokenlist, data.input, &i);
	}
	if (data.scanner_error)
	{
		ft_lstclear(&data.tokenlist, free_tokennode);
		return (NULL);
	}
	debug("====== token list successfully tokenized ====== ");
	return (data.tokenlist);
}

/*
*/
bool	got_tokens(t_mini_data *data, int *i)
{
	if (is_a_control_operator(data, i) || is_a_math_op(data, i) || \
	is_a_hist_expansion(data, i) || is_a_dollar_exp(data, i) || \
	is_some_semicolons(data, i) || is_a_redirection(data, i) || \
	is_simple_operator(data, i) || is_a_block(data, i) || \
	is_a_flag(data, i) || is_a_string_thing(data, i))
		return (true);
	else
		return (false);
}

/*
 * 
 */
int	skip_space(t_list *tokenlist, const char *input, int *i)
{
	t_list	*last;
	t_token	*lasttok;

	last = ft_lstlast(tokenlist);
	if (*i > 0 && last)
	{
		lasttok = get_curr_token(last);
		lasttok->folldbyspace = true;
		lasttok->howmany += 1;
	}
	while (is_space(input[*i]))
		advance(i);
	return (1);
}
