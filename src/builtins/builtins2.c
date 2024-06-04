/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 22:50:53 by rpriess           #+#    #+#             */
/*   Updated: 2024/05/28 22:51:10 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "libft.h"
#include "scanner.h"
#include "environment.h"
#include "history.h"
#include <unistd.h>

/*
Function merges the current and next token in a tokenlist:
- type of the first token gets preserved
- the token lexemes are joined together in a new lexeme
- the folldbyspace info from the second token is preserved
- the second token get deleted and the list node pointed to the following node in the list
*/
int merge_tokens_for_export(t_list *tokenlist)
{
    t_token *token_1;
    t_token *token_2;
    char    *new_lexeme;

    token_1 = get_curr_token(tokenlist);
    token_2 = get_curr_token(tokenlist->next);
    new_lexeme = ft_strjoin(token_1->lexeme, token_2->lexeme);
    free(token_1->lexeme);
    free(token_2->lexeme);
    token_1->folldbyspace = token_2->folldbyspace;
    token_1->lexeme = new_lexeme;
    if (tokenlist->next && tokenlist->next->next)
        tokenlist->next = tokenlist->next->next;
    if (consume_token(&(tokenlist->next)) && !token_followed_by_space(tokenlist))
        merge_tokens_for_export(tokenlist);
    return (0);
}