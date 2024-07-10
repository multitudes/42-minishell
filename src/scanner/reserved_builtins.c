/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:40:02 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/10 12:39:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"
#include "libft.h"


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
	if (peek(identifier, "while", EXACT))
		add_token(data, start, "while", WHILE);
	else if (peek(identifier, "until", EXACT))
		add_token(data, start, "until", UNTIL);
	else if (peek(identifier, "for", EXACT))
		add_token(data, start, "for", FOR);
	else if (peek(identifier, "case", EXACT))
		add_token(data, start, "case", CASE);
	else if (peek(identifier, "esac", EXACT))
		add_token(data, start, "esac", ESAC);
	else if (peek(identifier, "select", EXACT))
		add_token(data, start, "select", SELECT);
	else if (peek(identifier, "function", EXACT))
		add_token(data, start, "function", FUNCTION);
	else if (peek(identifier, "in", EXACT))
		add_token(data, start, "in", IN);
	else
		return (false);
	return (true);
}

bool	is_reserved2(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "if", EXACT))
		add_token(data, start, "if", IF);
	else if (peek(identifier, "then", EXACT))
		add_token(data, start, "then", THEN);
	else if (peek(identifier, "else", EXACT))
		add_token(data, start, "else", ELSE);
	else if (peek(identifier, "elif", EXACT))
		add_token(data, start, "elif", ELIF);
	else if (peek(identifier, "fi", EXACT))
		add_token(data, start, "fi", FI);
	else if (peek(identifier, "do", EXACT))
		add_token(data, start, "do", DO);
	else if (peek(identifier, "done", EXACT))
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
	if (!ft_strcmp(id, "bg") || !ft_strcmp(id, "fc") || !ft_strcmp(id, ":") \
	|| !ft_strcmp(id, "bind") || !ft_strcmp(id, "break") || \
	!ft_strcmp(id, "builtin") || !ft_strcmp(id, "caller") || \
	!ft_strcmp(id, "command") || !ft_strcmp(id, "compgen") || \
	!ft_strcmp(id, ".") || !ft_strcmp(id, "complete") || \
	!ft_strcmp(id, "continue") || !ft_strcmp(id, "declare") || \
	!ft_strcmp(id, "dirs") || !ft_strcmp(id, "disown") || \
	!ft_strcmp(id, "enable") || !ft_strcmp(id, "eval") || \
	!ft_strcmp(id, "exec") || !ft_strcmp(id, "alias") || \
	!ft_strcmp(id, "fg") || !ft_strcmp(id, "getopts") || \
	!ft_strcmp(id, "hash") || !ft_strcmp(id, "help") || \
	!ft_strcmp(id,"history") || !ft_strcmp(id, "jobs") || \
	!ft_strcmp(id, "kill") || !ft_strcmp(id, "let") || \
	!ft_strcmp(id, "local") || !ft_strcmp(id, "logout") || \
	!ft_strcmp(id, "mapfile") || !ft_strcmp(id, "popd") || \
	!ft_strcmp(id, "printf") || !ft_strcmp(id, "pushd") || \
	!ft_strcmp(id, "read") || !ft_strcmp(id, "readonly") || \
	!ft_strcmp(id, "return") || !ft_strcmp(id, "set") || \
	!ft_strcmp(id, "shift") || !ft_strcmp(id, "shopt") || \
	!ft_strcmp(id, "source") || !ft_strcmp(id, "suspend") || \
	!ft_strcmp(id, "test") || !ft_strcmp(id, "times") || \
	!ft_strcmp(id, "trap") || !ft_strcmp(id, "type") || \
	!ft_strcmp(id, "typeset") || !ft_strcmp(id, "ulimit") || \
	!ft_strcmp(id, "umask") || !ft_strcmp(id, "unalias") || \
	!ft_strcmp(id, "wait") || !ft_strcmp(id, "readarray"))
		return (true);
	return (false);
}


/*
is_builtin checks if the identifier is a builtin command
*/
bool	is_builtin(t_mini_data *data, char *identifier, int *start)
{
	if (peek(identifier, "echo", EXACT))
		add_token(data, start, "echo", BUILTIN);
	else if (peek(identifier, "cd", EXACT))
		add_token(data, start, "cd", BUILTIN);
	else if (peek(identifier, "export", EXACT))
		add_token(data, start, "export", BUILTIN);
	else if (peek(identifier, "unset", EXACT))
		add_token(data, start, "unset", BUILTIN);
	else if (peek(identifier, "env", EXACT))
		add_token(data, start, "env", BUILTIN);
	else if (peek(identifier, "exit", EXACT))
		add_token(data, start, "exit", BUILTIN);
	else if (peek(identifier, "pwd", EXACT))
		add_token(data, start, "pwd", BUILTIN);
	else if (not_implemented_builtin(identifier))
		add_token(data, start, identifier, BUILTIN);
	else
		return (false);
	return (true);
}
