/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 09:41:09 by rpriess           #+#    #+#             */
/*   Updated: 2024/07/05 20:07:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"
#include "parser.h"
#include "executer.h"
#include "error.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

static int  execute_heredoc_child(t_list *tokenlist, t_data *data, int pfd[2])
{
    char    **argv;

    if (close(pfd[1]) == -1)
        exit_and_print_err("minishell: error: parent close write end of pipe", 127);
    dup2(pfd[0], STDIN_FILENO);
    argv = get_argv_from_tokenlist(tokenlist);
    if (!resolve_command_path(argv, mini_get_env(data->env_arr, "PATH")))
        exit (127);
    debug("command and args: %s %s", argv[0], argv[1]);
    execve(argv[0], argv, (char **)data->env_arr->contents);
    return (exit_and_print_err(NULL, 127));
}

static int  execute_heredoc_parent(int pfd[2], pid_t pid, t_heredoc *heredoc)
{
    int     status;
    ssize_t write_return;

    status = 0;
    write_return = -1;
    if (close(pfd[0]) == -1)
        return(status_and_perror("minishell: error: parent close read end of pipe", 1));
    write_return = write(pfd[1], heredoc->buffer, ft_strlen(heredoc->buffer));
    if (write_return == -1 || (size_t)write_return != ft_strlen(heredoc->buffer))
        return(status_and_perror("minishell: write heredoc to stdin of command", 1));
    if (close(pfd[1]) == -1)
        return(status_and_perror("minishell: setup heredoc parent", 1));
    waitpid(pid, &status, 0);
    debug("status of my child %d", status);
    return (get_wait_status(status));
}

int redirect_and_execute_heredoc(t_ast_node *ast, t_data *data, t_heredoc *heredoc)
{
    int     pfd[2];
    pid_t   pid;
    int     status;

    debug("redirect and execute heredoc");
    status = 0;
    if (pipe(pfd) == -1)
        return(status_and_perror("minishell: setup heredoc pipe", 1));
    pid = fork();
    if (pid == -1)
        return(status_and_perror("minishell: redirect heredoc", 1));
    if (pid == 0)
        status = execute_heredoc_child(ast->token_list, data, pfd);
    else
        status = execute_heredoc_parent(pfd, pid, heredoc);
    return (status);
}
