/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:48:30 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/02 17:56:03 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "parser.h"
#include "error.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// refactor the redirect functions into one with one or two supporting functions

static void check_return(int new_fd, char *filename, uint8_t *status)
{
    if (new_fd < 0)
        *status = status_perror2("minishell: ", filename, 1);
}

static uint8_t dup2_by_redirect_type(t_tokentype type, char *filename, int *fd, u_int8_t *status)
{
    int new_fd;

    *status = 0;
    new_fd = -1;
    if (type == REDIRECT_OUT || type == REDIRECT_OUT_APP || type == DGREAT || type == CLOBBER)
        new_fd = dup2(*fd, STDOUT_FILENO);
    else if (type == REDIRECT_IN)
        new_fd = dup2(*fd, STDIN_FILENO);
    else if (type == REDIRECT_BOTH || type == REDIRECT_BOTH_APP)
    {
        new_fd = dup2(*fd, STDOUT_FILENO);
        new_fd = dup2(*fd, STDERR_FILENO);
    }
    // else if (type == REDIRECT_ERR)
    //     check_dup2_return(dup2(fd, STDERR_FILENO), filename, status);
    check_return(new_fd, filename, status);
    return (*status);
}

static int  open_fd_by_redirect_type(t_tokentype type, char *filename, uint8_t *status)
{
    int fd;

    fd = -1;
    if (type == REDIRECT_OUT || type == REDIRECT_BOTH || type == GREATER_AND || type == CLOBBER) // || type == REDIRECT_ERR
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    else if (type == REDIRECT_IN)
        fd = open(filename, O_RDONLY);
    else if (type == REDIRECT_OUT_APP || type == REDIRECT_BOTH_APP || type == DGREAT) // || type == REDIRECT_ERRAPP
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    check_return(fd, filename, status);
    return (fd);
}

/*
Opens and duplicates file used for redirection.
Deletes redirection and filename token.
*/
uint8_t setup_redirect(t_list **tokenlist, t_tokentype type)
{
    char    *filename;
    int     fd;
    uint8_t status;

    debug("setup redirect");
    fd = -1;
    status = 0;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    filename = get_token_lexeme(*tokenlist);
    fd = open_fd_by_redirect_type(type, filename, &status);
    if (status != 0)
        return (status);
    if (dup2_by_redirect_type(type, filename, &fd, &status) != 0)
        return (status);
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

static bool supported_redirect_token(t_tokentype type)
{
    if (type == REDIRECT_IN)
        return (true);
    else if (type == REDIRECT_OUT || type == REDIRECT_OUT_APP || type == DGREAT)
        return (true);
    else if (type == REDIRECT_BOTH || type == REDIRECT_BOTH_APP)
        return (true);
    return (false);
}

/*
Execute commands that contain basic redirection:
'<', '>', '>>'
*/
uint8_t	execute_redirection(t_ast_node **ast)
{
	t_list      *tokenlist;
    t_tokentype type;
    uint8_t     status;
    int         token_counter;

    debug("execute redirection");
    debug("ast type before redirection: %i", (*ast)->type);
    status = 0;
    token_counter = 0;
	tokenlist = (*ast)->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
    while (tokenlist)
    {
        type = get_token_type(tokenlist);
        if (supported_redirect_token(type))
            status = setup_redirect(&tokenlist, type);
        else
            token_counter++;
        if (status != 0)
            return (status);
        if (tokenlist)
            tokenlist = tokenlist->next;
    }
    if (tokenlist == NULL && token_counter == 0)
    {
        (*ast)->token_list = NULL;
        (*ast)->type = NODE_NULL;
    }
    debug("ast type after redirection: %i", (*ast)->type);
    return (status);
}
