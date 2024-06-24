/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpriess <rpriess@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:48:30 by rpriess           #+#    #+#             */
/*   Updated: 2024/06/24 15:20:51 by rpriess          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executer.h"
#include "parser.h"
#include "error.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int setup_redirect_stdout(t_list **tokenlist)
{
    char    *filename;
    int     fd;
    int     type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != 0 && type != 4) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDOUT_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

/*
Execute commands that contain basic redirection:
'<', '>', '>>'
*/
int	execute_redirection(t_ast_node *ast, t_data *data)
{
	t_list  *tokenlist;
    uint8_t status;

    debug("execute redirection");
	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
    while (tokenlist)
    {
        if (get_token_type(tokenlist) == REDIRECT_OUT)
            status = setup_redirect_stdout(&tokenlist);
        if (tokenlist)
            tokenlist = tokenlist->next;
    }
    ast->type = NODE_BUILTIN; // temp, could also be NODE_COMMAND
    status = execute_ast(ast, data); // I do not want to go through expansion again.
    return (status);
}
