/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/07/13 11:25:52 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "analyser.h"
#include "scanner.h"
#include <libft.h>
#include "splash_error.h"
#include "utils.h"
#include "builtins.h"

/*
Changes type of all tokens in string_tokens to WORD,
except for those $-tokens that our shell expands in a double quoted string
*/
static void	token_sanitization(t_list *string_tokens)
{
	while (string_tokens)
	{

		if (get_token_type(string_tokens) != VAR_EXPANSION \
			&& get_token_type(string_tokens) != DOLLAR_QUESTION)
			((t_token *)(string_tokens->content))->type = WORD;
		string_tokens = string_tokens->next;
	}
}

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

	tokenlist = ast->tokenlist;
	if (tokenlist == NULL || tokenlist->content == NULL)
		return ;
	token = (t_token *)tokenlist->content;
	if (token->type == BUILTIN)
	{
		ast->type = NODE_BUILTIN;
		return ;
	}
	// provisionally
	else if (token->type == PATHNAME \
			|| token->type == WORD || token->type == COMMAND)
	{
		ast->type = NODE_COMMAND;
		return ;
	}
	// else if (token->type == TRUETOK || token->type == FALSETOK)
	// {
	// 	debug("NODE_TRUE or NODE_FALSE");
	// }
}


/*
We dont expand tilde if the next char after the tilde is not a valid path
*/
bool	peek_is_valid_path(char c)
{
	if (ft_strchr("/:", c) || c == '\0')
		return (true);
	return (false);
}

/*
If there is a : or a first equal sign we expand.
If we are already past the first equal sign we dont expand if 
we have more equal signs in between.
*/
bool	valid_tilde_separator(char sep, int equal_status)
{
	if (ft_strchr(": ", sep))
		return (true);
	else if (sep == '=' && equal_status == 1)
		return (true);
	return (false);
}

/*
Replaces occurances of '~' in a string, if conditions for ~-expansion are met.
*/
char	*replace_tilde_in_str(t_list *tokenlist, char *lexeme, char *home, t_exp_flags *flags)
{
	char	*new_lexeme;
	int		i;
	char	*pos;
	char	*front;
	char	*back;
	char	*temp;

	i = 0;
	new_lexeme = ft_strdup(lexeme);
	if (flags->equal_status == 1)
	{
		pos = ft_strchr(new_lexeme, '=');
		if (!pos)
			pos = new_lexeme;
		else
			i = pos - new_lexeme + 1;
	}
	else
		pos = new_lexeme;
	while (new_lexeme && new_lexeme[i])
	{
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
	debug("replace_tilde_in_str %s with home: %s, new lexeme: %s", lexeme, home, new_lexeme);
	return (new_lexeme);
}

static char	*retrieve_home_from_cwd(char *cwd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cwd && cwd[i])
	{
		if (cwd[i] == '/')
			count++;
		i++;
		if (count == 3)
			break ;
	}
	if (count == 3 || (count == 2 && cwd[i] == '\0'))
		return (ft_strndup(cwd, i - 1));
	return (NULL);
	
}

/*
Get home from environment -  it is a tricky one
because bash always manages to get the home directory
even if HOME is not set in the environment
We are also not able to use special functions
so the best we can do is to check the user from 
the environment. In wtc/passwd the current user might also
not be included so it is not a viable option.
params:
env_array: if NULL or HOME is not set, get the home 
from the USER environment variable and collated with /home
Just pass NULL to the func ad you can get home from anywhere
but both need to be freed!
*/
char	*get_home(t_darray *env_arr)
{
	char	*home;
	char	buf[PATH_MAX];
	char	*cwd;

	home = mini_get_env(env_arr, "HOME");
	cwd = NULL;
	if (home && debug("HOME from env"))
		return (ft_strdup(home));
	else if (getenv("USER")  && debug("home from USER"))
		return (ft_strjoin("/home/", getenv("USER")));
	else
	{
		cwd = getcwd(buf, PATH_MAX);
		if (cwd && count_char_in_str(cwd, '/') > 1 && debug("HOME from cwd"))
			home = retrieve_home_from_cwd(cwd);
	}
	if (home)
		return (home);
	debug("HOME manually set to /home");
	return (ft_strdup("/home"));
}

/*
Checks if tilde expansion is valid based on next char in lexeme or next token
*/
bool	valid_tilde_expansion(t_list *tokenlist, int index)
{
	if (!tokenlist->next)
		return (true);
	else if (token_followed_by_space(tokenlist))
		return (true);
	else if (!token_followed_by_space(tokenlist) \
				&& peek_is_valid_path(get_token_lexeme(tokenlist)[index + 1]) \
				&& peek_is_valid_path((get_token_lexeme(tokenlist->next))[0]))
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

	token = get_curr_token(tokenlist);
	if (!token)
		return ;
	home = get_home(env_arr);
	if (token->type == TILDE && flags->equal_status == 1 \
		&& valid_tilde_expansion(tokenlist, 0))
		lexeme = home;
	else
	{
		lexeme = replace_tilde_in_str(tokenlist, token->lexeme, home, flags);
		free(home);
	}

	token->type = WORD;
	free(token->lexeme);
	token->lexeme = lexeme;
	debug("Expanded path token: %s", token->lexeme);
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
	if (end && (*end == ' ' || *end == '\0'))
		return (NULL);
	if (end && (*end == '?') && (*(end + 1) == '\0' \
			|| ((end + 1) && *(end + 1) == ' ')))
		return (ft_strdup("?"));
	if (end && (ft_isalnum(*end) || *end == '_'))
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
char	*replace_dollar_vars(t_data *data, char *lexeme)
{
	int		i;
	char	*new_lexeme;
	char	*key_value;
	char	*key;
	char	*temp[3];

	i = 0;
	new_lexeme = ft_strdup(lexeme);
	key = NULL;
	while (new_lexeme && new_lexeme[i])
	{
		key = get_key(new_lexeme + i);
		if (key != NULL)
		{
			if (ft_strlen(key) == 0)
				key_value = NULL;
			else if (ft_strcmp(key, "?") == 0 && data)
				key_value = ft_itoa(data->exit_status);
			else
				key_value = mini_get_env(data->env_arr, key);
			temp[0] = ft_strndup(new_lexeme, i);
			temp[1] = ft_strdup(new_lexeme + i + ft_strlen(key) + 1);
			temp[2] = new_lexeme;
			new_lexeme = ft_strjoin3(temp[0], key_value, temp[1]);
			i = i + ft_strlen(key_value);
			free(temp[0]);
			free(temp[1]);
			free(temp[2]);
			if (ft_strcmp(key, "?") == 0 && data)
				free (key_value);
			free(key);
			continue ;
		}
		i++;
	}
	return (new_lexeme);
}

/*
Used to expand variables indicated by "$".
*/
void	expand_dollar(t_data *data, t_token *token)
{
	char	*var;

	if (!token || !token->lexeme)
		return ;
	if (ft_strlen(token->lexeme) == 1 && token->lexeme[0] == '$')
		token->type = EXPANDED;
	else if (ft_strchr(token->lexeme, '$'))
	{
		var = replace_dollar_vars(data, token->lexeme);
		free(token->lexeme);
		token->lexeme = var;
		token->type = EXPANDED;
	}
	debug("Expand dollar, new token: %s, type: %i", token->lexeme, token->type);
}
/*
Expands $? to exit status
*/
void	expand_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	if (data)
		temp = ft_itoa(data->exit_status);
	else
		return ;
	if (token)
	{
		free(token->lexeme);
		token->lexeme = temp;
		token->type = WORD;
	}
	else
		free(temp);
}

void	expand_single_quotes(t_token *token)
{
	char	*lexeme;

	if (token && token->lexeme)
	{
		lexeme = ft_strtrim(token->lexeme, "'"); 
		free(token->lexeme);
		token->lexeme = lexeme;
		token->type = QUOTE_EXPANDED;
	}
	debug("Single quotes expanded token: %s, type: %i", token->lexeme, token->type);
}

void	expand_double_quotes(t_data *data, t_token *token)
{
	char	*unquoted_lexeme;
	char	*temp_lexeme;
	t_list	*string_tokens;
	t_list	*ptr_tokenlist;

	unquoted_lexeme = NULL;
	if (!token)
		return ;
	unquoted_lexeme = ft_strtrim(token->lexeme, "\""); // this does not behave well, when we have strings like "\"djklfjsdl\"" or ""dkldfj" as escape characters and unclosed quotes are not handled ;
	free(token->lexeme);
	token->lexeme = ft_strdup("");
	if (!ft_strchr(unquoted_lexeme, '$'))
	{
		token->lexeme = unquoted_lexeme;
		return ;
	}
	string_tokens = tokenizer(unquoted_lexeme);
	free(unquoted_lexeme);
	token_sanitization(string_tokens);
	ptr_tokenlist = string_tokens;
	while (string_tokens)
	{
		if (get_token_type(string_tokens) == VAR_EXPANSION || get_token_type(string_tokens) == DOLLAR_QUESTION)
			expand_dollar(data, get_curr_token(string_tokens));
		temp_lexeme = token->lexeme;
		if (((t_token *)(string_tokens->content))->folldbyspace == true)
			token->lexeme = ft_strjoin3(temp_lexeme, ((t_token *)string_tokens->content)->lexeme, " ");
		else
			token->lexeme = ft_strjoin(temp_lexeme, ((t_token *)string_tokens->content)->lexeme);
		free(temp_lexeme);
		string_tokens = string_tokens->next;
	}
	ft_lstclear(&ptr_tokenlist, free_tokennode);
	token->type = QUOTE_EXPANDED;
	debug("Double quotes expanded token: %s, type: %i", token->lexeme, token->type);
}

void	execute_expansion_by_type(t_data *data, t_list **tokenlist, t_exp_flags *flags)
{
	t_tokentype	type;

	// debug("Check and execute expansion by type (token type: %s)", get_token_type(tokenlist));
	type = get_token_type(*tokenlist);
	if (type == S_QUOTED_STRING)
		expand_single_quotes(get_curr_token(*tokenlist));
	else if (type == QUOTED_STRING)
		expand_double_quotes(data, get_curr_token(*tokenlist));
	else if (type == VAR_EXPANSION || type == DOLLAR \
			|| type == DOLLAR_QUESTION)
		expand_dollar(data, get_curr_token(*tokenlist));
	else if (ft_strchr(get_token_lexeme(*tokenlist), '~') \
			&& type != QUOTE_EXPANDED)
		expand_path(data->env_arr, *tokenlist, flags);
	else if (type == GLOBBING)
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
		{
			data->exit_status = \
				print_error_status("minishell: system error: missing token", 1);
			return ;
		}
		set_flags(tokenlist, &flags);
		execute_expansion_by_type(data, &tokenlist, &flags);
		if (token_followed_by_space(tokenlist))
			reset_flags(&flags);
		tokenlist = tokenlist->next;
	}
}

static bool	separated_token_types(t_list *tokenlist)
{
	t_tokentype	type_1;
	t_tokentype	type_2;

	if (!tokenlist || !tokenlist->next)
		return (true);
	type_1 = get_token_type(tokenlist);
	type_2 = get_token_type(tokenlist->next);
	if (is_redirection_token(type_1) || is_redirection_token(type_2))
		return (true);
	else if (is_heredoc_token(type_1) || is_heredoc_token(type_2))
		return (true);
	return (false);

}

/*
Expansion of nodes containing single quotes, double quotes, variables,
~ / paths, Special Parameters (only $? implemented)
Could be extended to also include other Special Parameters ($!, etc.),
$(..) ${..} $'..' $".."
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	t_list		*tokenlist;

	if (ast == NULL)
		return ;
	tokenlist = ast->tokenlist;
	if (!tokenlist)
		return ;
	expand_tokenlist(data, tokenlist);
	while (tokenlist && tokenlist->next)
	{
		if ((!token_followed_by_space(tokenlist) \
				&& !separated_token_types(tokenlist)) \
			|| (ft_strlen(get_token_lexeme(tokenlist)) == 0 \
				&& get_token_type(tokenlist) != QUOTE_EXPANDED))
			merge_tokens(tokenlist);
		else
			tokenlist = tokenlist->next;
	}
	which_ast_node(ast);
	debug("First lexeme in expanded and merged tokenlist: -%s-, type: %i, ast-node type: %i", get_token_lexeme(ast->tokenlist), get_token_type(ast->tokenlist),  ast->type);
}
