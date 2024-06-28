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
    type = get_token_type(*tokenlist);
    if (type != WORD && type != PATHNAME) // add all types that could be expanded to a filename
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
uint8_t	execute_redirection(t_ast_node *ast, t_data *data)
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
        else if (get_token_type(tokenlist) == REDIRECT_IN)
            status = setup_redirect_stdin(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_OUT_APP)
            status = setup_redirect_stdoutapp(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_BOTH)
            status = setup_redirect_stdouterr(&tokenlist);
        else if (get_token_type(tokenlist) == REDIRECT_BOTH_APP)
        {
            status = setup_redirect_stdoutapp(&tokenlist);
            status = setup_redirect_stderrapp(&tokenlist);
        }
        if (status != 0)
            return (status);
        if (tokenlist)
            tokenlist = tokenlist->next;
    }
    status = execute_ast(ast, data); // I do not want to go through expansion again.
    return (status);
}
