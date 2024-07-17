/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:53:34 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/17 13:49:29 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "splash.h"
#include "scanner.h"
#include "parser.h"
#include "utils.h"
#include "splash_error.h"
#include "debug.h"
#include "libft.h"
#include <stdbool.h>

static char	*remove_sign_token(t_list **tokenlist)
{
	t_list	*tmp;
	t_list	*prev;
	char	*sign;

	if (tokenlist == NULL || *tokenlist == NULL)
		return (false);
	if (ft_strncmp(get_token_lexeme(*tokenlist), "-", 1) == 0)
		sign = ft_strdup("-");
	else
		sign = ft_strdup("+");
	tmp = *tokenlist;
	prev = tmp->prev;
	*tokenlist = tmp->next;
	if (tmp->next)
		tmp->next->prev = prev;
	free_tokennode(tmp->content);
	free(tmp);
	return (sign);
}

static void	merge_sign_token(t_list **tokenlist)
{
	char	*old_lexeme;
	char	*new_lexeme;
	char	*sign;

	old_lexeme = get_token_lexeme(*tokenlist);
	if (!old_lexeme)
		return ;
	if ((old_lexeme[0] == '-' || old_lexeme[0] == '+') \
		&& ft_strlen(old_lexeme) == 1)
	{
		sign = remove_sign_token(tokenlist);
		old_lexeme = get_token_lexeme(*tokenlist);
		new_lexeme = ft_strjoin(sign, old_lexeme);
		free(sign);
		free(old_lexeme);
		if (!new_lexeme)
			return ;
		((t_token *)(*tokenlist)->content)->lexeme = new_lexeme;
	}
	return ;
}

/*
Now updated as follows
if I have a "exit -100" I will have the 3 tokens exit - 100
so I do atoi of 100 and multiply to -1 and cast to unsigned int 
modulo 256 to get the same result as bash
*/
uint8_t	execute_exit_builtin(t_data *data, t_list *tokenlist)
{
	uint8_t	status;
	char	*lexeme;
	t_list	*head;

	head = tokenlist;
	tokenlist = tokenlist->next;
	merge_sign_token(&tokenlist);
	head->next = tokenlist;
	lexeme = get_token_lexeme(tokenlist);
	if (lexeme && ft_isnumstring(lexeme) && tokenlist->next)
		return (stderr_and_status("exit: too many arguments", 1));
	ft_write(1, "exit\n");
	if (lexeme && ft_isnumstring(lexeme))
		status = (unsigned int)ft_atoi(lexeme) % 256;
	else if (lexeme && !ft_isnumstring(lexeme))
		status = stderr_and_status3("exit: ", lexeme, \
									": numeric argument required", 2);
	else
		status = data->exit_status;
	free_ast(&(data->ast));
	free((char *)(data->input));
	free_data(&data);
	free(data);
	debug("Exit status: %i", (int)status);
	exit(status);
}
