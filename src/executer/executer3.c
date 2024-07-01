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

// refactor the redirect functions into one with one or two supporting functions

uint8_t setup_redirect_stderrapp(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDERR_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

uint8_t setup_redirect_stdouterr(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDOUT_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDERR_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

uint8_t setup_redirect_stderr(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDERR_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

uint8_t setup_redirect_stdoutapp(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDIN_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

uint8_t setup_redirect_stdin(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    if (dup2(fd, STDIN_FILENO) < 0)
        return (status_and_detailed_perror("minishell: ", filename, 1));
    consume_token_and_connect(tokenlist);
    close(fd);
    return (0);
}

uint8_t setup_redirect_stdout(t_list **tokenlist)
{
    char        *filename;
    int         fd;
    t_tokentype type;

    debug("setup redirect stdout");
    filename = NULL;
    consume_token_and_connect(tokenlist);
    if (!(*tokenlist))
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme 'newline'
    // print_token_list(*tokenlist);
    // *tokenlist = (*tokenlist)->next;
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename ; maybe use access() to also ensure it is not a directory etc
        return (print_minishell_error_status("minishell: syntax error near unexpected token ", 2)); // add token lexeme
    filename = get_token_lexeme(*tokenlist);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
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
uint8_t	execute_redirection(t_ast_node **ast)
{
	t_list  *tokenlist;
    uint8_t status;
    int     token_counter;

    debug("execute redirection");
    debug("ast type before redirection: %i", (*ast)->type);
    status = 0;
    token_counter = 0;
	tokenlist = (*ast)->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return (0);
    while (tokenlist)
    {
        if (get_token_type(tokenlist) == REDIRECT_OUT)
            status = setup_redirect_stdout(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_IN)
            status = setup_redirect_stdin(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_OUT_APP)
            status = setup_redirect_stdoutapp(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_BOTH)
            status = setup_redirect_stdouterr(&tokenlist);
        // else if (get_token_type(tokenlist) == REDIRECT_BOTH_APP) // this should require some more specific treatment of token deletion
        // {
        //     status = setup_redirect_stdoutapp(&tokenlist);
        //     status = setup_redirect_stderrapp(&tokenlist);
        // }
        else
            token_counter++;
        if (status != 0)
            return (status);
        if (tokenlist)
            tokenlist = tokenlist->next;
    }
    debug("Pointer to tokenlist after processing redirection: %p", tokenlist);
    if (tokenlist == NULL && token_counter == 0)
    {
        (*ast)->token_list = NULL;
        (*ast)->type = NODE_NULL;
    }
    debug("ast type after redirection: %i", (*ast)->type);
    // if (tokenlist == NULL && tokenlist->prev == NULL)
    //     (*ast)->token_list = NULL;
    // else if (tokenlist == NULL)
    //     (*ast)->token_list->next = NULL;
    // else
    //     ft_lstadd_back(&(*ast)->token_list, tokenlist);
    // print_token_list((*ast)->token_list);
    return (status);
}
