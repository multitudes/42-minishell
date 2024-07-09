/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:20:28 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/06 18:03:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "analyser.h"
#include "scanner.h"
#include "builtins.h"
#include "utils.h"
#include "splash_error.h"
#include <libft.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h> // needed?

/*
removes all single and double qoutes from a string.
*/
static void remove_quotes(char *string)
{
    int     i;
    int     j;

    i = 0;
    j = 0;
    while (string && string[i])
    {        
        if (string[i] != '"' && string[i] != '\'')
        {
            string[j] = string[i];
            j++;
        }
        i++;
    }
    string[j] = '\0';
}

/*
Checks for and handles single/double quotes in heredoc delimiter.
*/
static bool process_delim_quotes(t_heredoc *heredoc)
{
    int count_single_quotes;
    int count_double_quotes;
    int i;

    count_single_quotes = 0;
    count_double_quotes = 0;
    i = 0;
    while (i < heredoc->delim_count)
    {
        count_single_quotes = count_char_in_str(heredoc->delim[i], '\'');
        count_double_quotes = count_char_in_str(heredoc->delim[i], '"');
        if (count_single_quotes % 2 || count_double_quotes % 2)
            return (false_and_print("minishell: heredoc: invalid delimiter"));
        else if (count_single_quotes > 1 || count_double_quotes > 1)
        {
            remove_quotes(heredoc->delim[i]);
            heredoc->expansion[i] = false;
        }
        else
            heredoc->expansion[i] = true;
        i++;
        count_single_quotes = 0;
        count_double_quotes = 0;
    }
    i = 0;
    return (true);
}

/*
Set up heredoc(s). Up to 10 heredocs/delimiters are supported.
*/
static bool init_heredoc(t_ast_node *ast, t_heredoc *heredoc)
{
    t_list  *tokenlist;

    heredoc->buffer_size = HEREDOC_BUFFER;
    heredoc->delim_count = 0;
    heredoc->heredoc_len = 0;
    tokenlist = ast->token_list;
    while (tokenlist)
    {
        if (get_token_type(tokenlist) == DLESS)
        {
            if (tokenlist->next && get_token_type(tokenlist->next) == DLESS_DELIM)
            {
                heredoc->delim[heredoc->delim_count] = ft_strdup(get_token_lexeme(tokenlist->next));
				if (!heredoc->delim[heredoc->delim_count])
				{	
					free_heredoc(heredoc);
					return false_and_print("minishell: heredoc memory error");
				}
				debug("Set delimiter %i: %s", heredoc->delim_count, heredoc->delim[heredoc->delim_count]);
                heredoc->delim_count++;
                consume_token_and_connect(&tokenlist);
                consume_token_and_connect(&tokenlist);
                continue ;
            }
            else
                return(false_and_print("minishell: syntax error"));
        }
        tokenlist = tokenlist->next;
    }
    debug("init heredoc complete, delimiter count: %i", heredoc->delim_count);
    return (true);
}

/*
Executes heredoc and then passes heredoc to stdin of system command
or calls builtin functions depending on ast node type.
*/
int execute_heredoc(t_ast_node *ast, t_data *data)
{
    t_heredoc   heredoc;
    int         status;

    debug("execute heredoc");
    set_up_heredoc_signals();
    status = 1;
    if (!init_heredoc(ast, &heredoc) || !process_delim_quotes(&heredoc) || !process_heredoc(&heredoc, data))
    {
        set_up_std_signals();
        return (status);
    }
    if ((ast->type == NODE_COMMAND || ast->type == NODE_TERMINAL) && only_flags(ast->token_list->next) && heredoc.buffer)
        status = redirect_and_execute_heredoc(ast, data, &heredoc);
    else if (ast->type == NODE_COMMAND || ast->type == NODE_TERMINAL)
        status = execute_command(ast->token_list, data);
    else if (ast->type == NODE_BUILTIN)
		status = execute_builtin(ast->token_list, data);
	free(heredoc.buffer);
    set_up_std_signals();
    return (status);
}

/*
Checks for heredoc token in tokenlist.
*/
bool    is_heredoc(t_list *tokenlist)
{
    t_tokentype tokentype;

    while (tokenlist)
    {
        tokentype = get_token_type(tokenlist);
        if (is_heredoc_token(tokentype))
            return (true);
        tokenlist = tokenlist->next;
    }
    return (false);
}
