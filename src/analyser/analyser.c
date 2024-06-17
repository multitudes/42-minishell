/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/17 16:13:55 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "analyser.h"
#include "scanner.h"
#include <libft.h>
#include "error.h"
#include "utils.h"
#include "builtins.h"

/*
it checks if the terminal ast node is a builtin or a command
but this could be already done in the parser 🧐🤨
This function is probably just for debug purposes
need to check that the parser interprets the tokens correctly
*/
void	which_ast_node(t_ast_node *ast)
{
	t_list *tokenlist;
	t_token *token;

	tokenlist = ast->token_list;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	token = (t_token *)tokenlist->content;
	debug("which_ast_node");
	if (token->type == BUILTIN)
	{
		ast->type = NODE_BUILTIN;
		debug("NODE_BUILTIN");
		return ;
	}
	// provisionally
	else if (token->type == PATHNAME || token->type == WORD)
	{
		ast->type = NODE_COMMAND;
		debug("NODE_COMMAND");
		return ;
	}
	else if (token->type == TRUETOK || token->type == FALSETOK)
	{
		debug("NODE_TRUE or NODE_FALSE");
	}
	else
		debug("not TERMINAL");
}


/*
We dont expand tilde if the next char after the tilde is not a valid path
*/
int	peek_is_valid_path(char c)
{
	if (ft_strchr("/:", c) || c == '\0')
		return (1);
	return (0);
}

/*
If there is a : or a first equal sign we expand.
If we are already past the first equal sign we dont expand if 
we have more equal signs in between.
*/
bool	valid_tilde_separator(char sep, int equal_status)
{
	if (ft_strchr(":", sep))
		return (true);
	else if (sep == '=' && equal_status == 1)
		return (true);
	return (false);
}

/*
*/
char	*replace_tilde_in_str(t_list *tokenlist, char *lexeme, char *home, t_exp_flags *flags)
{
	char	*new_lexeme;
	int		i;
	char	*pos;
	char	*front;
	char	*back;
	char	*temp;

	debug("replace_tilde_in_str");
	i = 0;
	new_lexeme = ft_strdup(lexeme);
	if (flags->equal_status == 1)
	{
		pos = ft_strchr(lexeme, '=');
		if (!pos)
		{
			pos = lexeme;
			flags->equal_status = 2;
		}
		i = pos - lexeme + 1;
	}
	else
		pos = lexeme;
	while (new_lexeme && new_lexeme[i])
	{
		debug("State of equal_status flag: %i", flags->equal_status);
		if (new_lexeme[i] == '~' && i == 0 && peek_is_valid_path(new_lexeme[i + 1]) && (flags->equal_status == 1) && valid_tilde_expansion(tokenlist, i))
		{
			back = ft_strdup(new_lexeme + 1);
			temp = new_lexeme;
			new_lexeme = ft_strjoin(home, back);
			free(back);
			free(temp);
			i = ft_strlen(home) - 1;
		}
		else if (new_lexeme[i] == '~' && peek_is_valid_path(new_lexeme[i + 1]) && (i != 0 && valid_tilde_separator(new_lexeme[i - 1], flags->equal_status)) && valid_tilde_expansion(tokenlist, i))
		{
			front = ft_strndup(new_lexeme, i);
			back = ft_strdup(new_lexeme + i + 1);
			if (flags->equal_status == 1)
				flags->equal_status = 2;
			temp = new_lexeme;
			debug("front of new string: %s", front);
			debug("middle of new string: %s", home);
			debug("back of new string: %s", back);
			new_lexeme = ft_strjoin3(front, home, back);
			free(front);
			free(back);
			free(temp);
			i = i + ft_strlen(home) - 1;
		}
		if (new_lexeme[i] == '=' && flags->equal_status == 1)
			flags->equal_status = 2;
		i++;
	}
	return (new_lexeme);
}

/*
Get home from environment
TODO (tbc) in the future: if HOME is unset,
get HOME through system configuration entries.
*/
char	*get_home(t_darray *env_arr)
{
	char	*home;

	home = ft_strdup(mini_get_env(env_arr, "HOME"));
	debug("'HOME' retrieved: %s", home);
	return (home);
}

bool	valid_tilde_expansion(t_list *tokenlist, int index)
{
	if (!tokenlist->next)
		return (true);
	else if (token_followed_by_space(tokenlist))
		return (true);
	else if (!token_followed_by_space(tokenlist) && peek_is_valid_path(get_token_lexeme(tokenlist)[index + 1]) && peek_is_valid_path((get_token_lexeme(tokenlist->next))[0]))
		return (true);
	return (false);
}

/*
Expands "~" in pathnames
*/
void	expand_path(t_darray *env_arr, t_list *tokenlist, t_exp_flags *flags)
{
	char	*lexeme;
	char	*home;
	t_token	*token;

	debug("expand_path");
	token = get_curr_token(tokenlist);
	if (!token)
		return ;
	home = get_home(env_arr);
	if (token->type == TILDE && flags->equal_status == 1 && valid_tilde_expansion(tokenlist, 0))
		lexeme = home;
	else
	{
		lexeme = replace_tilde_in_str(tokenlist, token->lexeme, home, flags);
		free(home);
	}
//	token->type = WORD;
	free(token->lexeme);
	token->lexeme = lexeme;
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

/*
Looks at next char in str. If it is a $,
then the potential key/var-name is returned.
'$' followed by invalid var-syntax, returns an empty string.
Returned key-string needs to be freed.
*/
char	*get_key(char *str)
{
	char	*end;

	if (!str)
		return (NULL);
	if (*str != '$')
		return (NULL);
	end = str + 1;
	if (end && (ft_isalpha(*end) || *end == '_'))
		end++;
	else
		return (ft_strdup(""));
	while (end && (ft_isalnum(*end) || *end == '_'))
		end++;
	return (ft_strndup(str + 1, (size_t)(end - (str + 1))));
}

/*
In a token lexeme replaces all $-indicated variables
with their respective values or with empty string if not existing.
Returns the new lexeme, which must be freed.
*/
char	*replace_dollar_vars(t_darray *env_arr, char *lexeme)
{
	int		i;
	char	*new_lexeme;
	char	*key_value;
	char	*key;
	char	*temp[3];

	debug("replace dollar vars");
	i = 0;
	new_lexeme = ft_strdup(lexeme);
	key = NULL;
	while (new_lexeme && new_lexeme[i])
	{
		key = get_key(new_lexeme + i);
		debug("Extracted key: %s", key);
		if (key != NULL)
		{
			if (ft_strlen(key) == 0)
				key_value = NULL;
			else
				key_value = mini_get_env(env_arr, key);
			temp[0] = ft_strndup(new_lexeme, i);
			temp[1] = ft_strdup(new_lexeme + i + ft_strlen(key) + 1);
			temp[2] = new_lexeme;
			new_lexeme = ft_strjoin3(temp[0], key_value, temp[1]);
			i = i + ft_strlen(key_value);
			free(temp[0]);
			free(temp[1]);
			free(temp[2]);
			free(key);
			break ;
		}
		i++;
	}
	return (new_lexeme);
}

/*
Used to expand variables, indicated by "$".
*/
void	expand_dollar(t_darray *env_arr, t_token *token)
{
	char	*var;

	debug("expand_dollar");
	if (!token)
		return ;
//	token->type = WORD;
	if (token->lexeme && ft_strchr(token->lexeme, '$'))
	{
		var = replace_dollar_vars(env_arr, token->lexeme);
		free(token->lexeme);
		token->lexeme = var;
	}
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

void	expand_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	debug("expand_exit_status");
	if (!data)
		temp = ft_itoa(data->exit_status);
	else
		return ;
	if (token)
	{
		free(token->lexeme);
		token->lexeme = temp;
//		token->type = WORD;
	}
}

void	expand_single_quotes(t_token *token)
{
	char	*lexeme;

	debug("extract_string");
	if (token && token->lexeme)
	{
		lexeme = ft_strtrim(token->lexeme, "'"); 
		free(token->lexeme);
		token->lexeme = lexeme;
//		token->type = WORD;
	}
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

void	expand_double_quotes(t_data *data, t_token *token)
{
	char	*unquoted_lexeme;
	char	*temp_lexeme;
	t_list	*string_tokens;
	t_list	*ptr_token_list;

	debug("expand_string");
	unquoted_lexeme = NULL;
	if (!token)
		return ;
	unquoted_lexeme = ft_strtrim(token->lexeme, "\""); // this does not behave well, when we have strings like "\"djklfjsdl\"" or ""dkldfj" as escape characters and unclosed quotes are not handled ;
	free(token->lexeme);
	token->lexeme = NULL;
	if (!ft_strchr(unquoted_lexeme, '$'))
	{
		token->lexeme = unquoted_lexeme;
		return ;
	}
	string_tokens = string_tokenizer(unquoted_lexeme);
	free(unquoted_lexeme);
	ptr_token_list = string_tokens;
	while (string_tokens)
	{
		if (get_token_type(string_tokens) == VAR_EXPANSION || get_token_type(string_tokens) == DOLLAR)
			expand_dollar(data->env_arr, get_curr_token(string_tokens));
		else if (get_token_type(string_tokens) == DOLLAR_QUESTION)
			expand_exit_status(data, get_curr_token(string_tokens));
		temp_lexeme = token->lexeme;
		token->lexeme = ft_strjoin(temp_lexeme, ((t_token *)string_tokens->content)->lexeme);
		free(temp_lexeme);
		string_tokens = string_tokens->next;
	}
	ft_lstclear(&ptr_token_list, free_tokennode); //free_tokennode function is from scanner.h
//	token->type = WORD;
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

void	execute_expansion_by_type(t_data *data, t_list *tokenlist, t_exp_flags *flags)
{
	if (get_token_type(tokenlist) == S_QUOTED_STRING)
		expand_single_quotes(get_curr_token(tokenlist));
	else if (get_token_type(tokenlist) == QUOTED_STRING)
		expand_double_quotes(data, get_curr_token(tokenlist));
	else if (get_token_type(tokenlist) == VAR_EXPANSION || get_token_type(tokenlist) == DOLLAR)
		expand_dollar(data->env_arr, get_curr_token(tokenlist));
	else if (ft_strchr(get_token_lexeme(tokenlist), '~'))
		expand_path(data->env_arr, tokenlist, flags);
//	else if (get_token_type(tokenlist) == TILDE || get_token_type(tokenlist) == PATHNAME)
//		expand_path(data->env_arr, tokenlist, flags);
	else if (get_token_type(tokenlist) == DOLLAR_QUESTION)
		expand_exit_status(data, get_curr_token(tokenlist));
	else if (get_token_type(tokenlist) == GLOBBING)
		expand_globbing(tokenlist);
}

/*
Identify if and which expansion needed and call the appropriate expansion function.
*/
void	expand_tokenlist(t_data *data, t_list *tokenlist)
{
	t_exp_flags	flags;

	reset_flags(&flags);
	while (tokenlist)
	{
		if (!get_curr_token(tokenlist))
			print_error_status("minishell: system error: missing token\n", 1);
		debug("Token to check for expansion - token type: %d, lexeme: %s", get_token_type(tokenlist), get_token_lexeme(tokenlist));
		set_flags(tokenlist, &flags);
		execute_expansion_by_type(data, tokenlist, &flags);
		if (token_followed_by_space(tokenlist))
			reset_flags(&flags);
		tokenlist = tokenlist->next;
	}
}

/*
Expansion of nodes containing single quotes, double quotes, variables,
~ / paths, Special Parameters (only $? implemented)
Could be extended to also include other Special Parameters ($!, etc.),
$(..) ${..} $'..' $".."
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	t_list	*tokenlist;

	if (ast == NULL)
		return ;
	tokenlist = ast->token_list;
	debug("analyse expand");
	which_ast_node(ast);
	debug("AST type: %d", ast->type);
	expand_tokenlist(data, tokenlist);
	while (tokenlist->next && !token_followed_by_space(tokenlist))
		merge_tokens(tokenlist);
	// debug("---------left -----------");
	// if (ast->left)
	// 	analyse_expand(ast->left, data);
	// else
	// 	debug("left is NULL");
	// debug("---------right -----------");
	// if (ast->right)
	// 	analyse_expand(ast->right, data);
	// else
	// 	debug("right is NULL");
}
