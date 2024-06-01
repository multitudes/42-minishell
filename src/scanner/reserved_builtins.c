/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:40:02 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/01 11:34:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 mh.. just checked again in the bash and maybe the function below is not 
 necessary? TODO
 bash lets me create a var while and print it with $while... 	
 so not for identifier.  they cannot be used for commands...!
 IF, THEN, ELSE, ELIF, FI, DO, DONE, WHILE, UNTIL, FOR, CASE, ESAC, SELECT,
  FUNCTION,
*/
bool	is_reserved1(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "while", true))
		add_token(data, start, "while", WHILE);
	else if (peek(identifier, "until", true))
		add_token(data, start, "until", UNTIL);
	else if (peek(identifier, "for", true))
		add_token(data, start, "for", FOR);
	else if (peek(identifier, "case", true))
		add_token(data, start, "case", CASE);
	else if (peek(identifier, "esac", true))
		add_token(data, start, "esac", ESAC);
	else if (peek(identifier, "select", true))
		add_token(data, start, "select", SELECT);
	else if (peek(identifier, "function", true))
		add_token(data, start, "function", FUNCTION);
	else if (peek(identifier, "in", true))
		add_token(data, start, "in", IN);
	else
		return (false);
	return (true);
}

bool	is_reserved2(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "if", true))
		add_token(data, start, "if", IF);
	else if (peek(identifier, "then", true))
		add_token(data, start, "then", THEN);
	else if (peek(identifier, "else", true))
		add_token(data, start, "else", ELSE);
	else if (peek(identifier, "elif", true))
		add_token(data, start, "elif", ELIF);
	else if (peek(identifier, "fi", true))
		add_token(data, start, "fi", FI);
	else if (peek(identifier, "do", true))
		add_token(data, start, "do", DO);
	else if (peek(identifier, "done", true))
		add_token(data, start, "done", DONE);
	else
		return (false);
	return (true);
}

bool	is_reserved(t_mini_data *data, char *identifier, int *start)
{
	if (is_reserved1(data, identifier, start))
		return (true);
	else if (is_reserved2(data, identifier, start))
		return (true);
	else
		return (false);
}

/*
bash builtins which we do not implement are in this list (taken from the 
bash manual but enhanced by copilot)
"alias bg bind break builtin caller command compgen 
. : complete continue declare dirs disown enable eval exec fc fg getopts hash 
help history jobs kill let local logout mapfile popd printf pushd read 
readonly return set shift shopt source suspend test times trap type 
typeset ulimit umask unalias wait readarray"
*/
bool	not_implemented_builtin(const char *id)
{
	if (!ft_strncicmp(id, "bg", 3) || !ft_strncicmp(id, "fc", 2) || !ft_strncicmp(id, \
	":", 2) || !ft_strncicmp(id, "bind", 5) || !ft_strncicmp(id, "break", 6) || \
	!ft_strncicmp(id, "builtin", 8) || !ft_strncicmp(id, "caller", 6) || \
	!ft_strncicmp(id, "command", 7) || !ft_strncicmp(id, "compgen", 7) || \
	!ft_strncicmp(id, ".", 2) || !ft_strncicmp(id, "complete", 9) || !ft_strncicmp(id, \
	"continue", 8) || !ft_strncicmp(id, "declare", 7) || !ft_strncicmp(id, "dirs", \
	4) || !ft_strncicmp(id, "disown", 6) || !ft_strncicmp(id, "enable", 6) || \
	!ft_strncicmp(id, "eval", 4) || !ft_strncicmp(id, "exec", 4) || !ft_strncicmp(id, \
	"alias", 6) || !ft_strncicmp(id, "fg", 2) || !ft_strncicmp(id, "getopts", 7) || \
	!ft_strncicmp(id, "hash", 4) || !ft_strncicmp(id, "help", 4) || !ft_strncicmp(id, \
	"history", 7) || !ft_strncicmp(id, "jobs", 4) || !ft_strncicmp(id, "kill", 4) || \
	!ft_strncicmp(id, "let", 3) || !ft_strncicmp(id, "local", 5) || !ft_strncicmp(id, \
	"logout", 6) || !ft_strncicmp(id, "mapfile", 7) || !ft_strncicmp(id, "popd", 4) \
	|| !ft_strncicmp(id, "printf", 6) || !ft_strncicmp(id, "pushd", 5) || \
	!ft_strncicmp(id, "read", 4) || !ft_strncicmp(id, "readonly", 8) || \
	!ft_strncicmp(id, "return", 6) || !ft_strncicmp(id, "set", 3) || \
	!ft_strncicmp(id, "shift", 5) || !ft_strncicmp(id, "shopt", 5) || \
	!ft_strncicmp(id, "source", 6) || !ft_strncicmp(id, "suspend", 7) || \
	!ft_strncicmp(id, "test", 4) || !ft_strncicmp(id, "times", 5) || \
	!ft_strncicmp(id, "trap", 4) || !ft_strncicmp(id, "type", 4) || \
	!ft_strncicmp(id, "typeset", 7) || !ft_strncicmp(id, "ulimit", 6) || \
	!ft_strncicmp(id, "umask", 5) || !ft_strncicmp(id, "unalias", 7) || \
	!ft_strncicmp(id, "wait", 4) || !ft_strncicmp(id, "readarray", 9))
		return (true);
	return (false);
}

/*
is_builtin checks if the identifier is a builtin command
*/
bool	is_builtin(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "echo", true))
		add_token(data, start, "echo", BUILTIN);
	else if (peek(identifier, "cd", true))
		add_token(data, start, "cd", BUILTIN);
	else if (peek(identifier, "export", true))
		add_token(data, start, "export", BUILTIN);
	else if (peek(identifier, "unset", true))
		add_token(data, start, "unset", BUILTIN);
	else if (peek(identifier, "env", true))
		add_token(data, start, "env", BUILTIN);
	else if (peek(identifier, "exit", true))
		add_token(data, start, "exit", BUILTIN);
	else if (peek(identifier, "pwd", true))
		add_token(data, start, "pwd", BUILTIN);
	else if (not_implemented_builtin(identifier))
		add_token(data, start, identifier, BUILTIN);
	else
		return (false);
	return (true);
}
