/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:37:45 by lbrusa            #+#    #+#             */
/*   Updated: 2024/06/04 07:46:06 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "analyser.h"
#include "scanner.h"

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


void	expand_globbing(t_list *tokenlist)
{
	char	*pat;
	t_darray	*files;
	
	// debug("expand_globbing");
	debug("token type: %d lexeme: %s", get_token_type(tokenlist), get_token_lexeme(tokenlist));
	pat = get_token_lexeme(tokenlist);
	files = darray_create(sizeof(char *), 100);
	if (match_files_in_directory(files, pat))
	{
		debug("files count : %d", files->end);
		
		t_list *next = tokenlist->next;
		t_list *head = tokenlist->prev;
		head->next = NULL;
		tokenlist->next = NULL;
		tokenlist->prev = NULL;
		debug("head still: %s and next %s", get_token_lexeme(head), get_token_lexeme(next));
		// create a new linked list of tokens with the file names
		int i = 0;
		int start = 0;
		while (i < files->end)
		{
			char *file = darray_get(files, i);
			debug("file: %s", file);
			t_list *new_node = new_toknode(WORD, file, &start);
			debug("new node: %s", get_token_lexeme(new_node));
			ft_lstadd_back(&head, new_node);
			i++;
		}
		debug("head still: %s and next %s", get_token_lexeme(head), get_token_lexeme(head->next));
		t_list *last = ft_lstlast(head);
		last->next = next;
		debug("last still: %s and next %s", get_token_lexeme(last), get_token_lexeme(next));
		if (next)
			next->prev = last;
		// free the old list
		debug("tokenlist %s and next %s", get_token_lexeme(tokenlist), get_token_lexeme(tokenlist));
		// ft_lstclear(&tokenlist, free_tokennode);
		// ft_lstdelone(tokenlist, free_tokennode);
		// free the darray
	}
		darray_clear_destroy(files);
	return ;
}

int	count_chars_in_str(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

int	peek_is_valid_path(char c)
{
	if (ft_strchr("\\", c) || c == '\0')
		return (1);
	return (0);
}

int	valid_tilde_separator(char sep, int	equal_valid)
{
	if (ft_strchr(":", sep))
		return (1);
	else if (sep == '=' && equal_valid == 1)
		return (1);
	return (0);
}

char	*replace_tilde_in_str(char *str, char *home)
{
	char	*new_str;
	int		i;
	int		equal_sep;
	char	*pos;
	char	*front;
	char	*back;
	char	*temp;

//tilde before '=' will not get replaced.
//tilde after '=' or when no '=' in string will get replaced, if at first position and valid path char at next position ("/") or end of string
// or after ":" if also after '=' if valid path char at next position ("/", ":") or end of string
	i = 0;
	equal_sep = 1;
	new_str = ft_strdup(str);
	pos = ft_strchr(str, '=');
	if (!pos)
	{
		pos = str;
		equal_sep = 0;
	}
	else
		i = pos - str + 1;
	while (new_str && new_str[i])
	{
		if (new_str[i] == '~' && i == 0 && peek_is_valid_path(new_str[i + 1]))
		{
			back = ft_strdup(new_str + 1);
			temp = new_str;
			new_str = ft_strjoin(home, back);
			free(back);
			free(temp);
			i = ft_strlen(home) - 1;
		}
		else if (new_str[i] == '~' && peek_is_valid_path(new_str[i + 1]) && i != 0 && valid_tilde_separator(new_str[i - 1], equal_sep))
		{
			front = ft_strndup(new_str, i);
			back = ft_strdup(new_str + i + 1);
			equal_sep = 0;
			temp = new_str;
			new_str = ft_strjoin3(front, home, back);
			free(front);
			free(back);
			free(temp);
			i = i + ft_strlen(home) - 1;
		}
		i++;
	}
	return (new_str);
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
	return (home);
}

/*
Expands "~" in pathnames
*/
void	expand_path(t_darray *env_arr, t_token *token)
{
	char	*lexeme;
	char	*home;

	debug("expand_path");
	if (!token)
		return ;
	home = get_home(env_arr);
	if (token->type == TILDE)
		lexeme = home;
	else if (token->type == PATHNAME)
	{
		lexeme = replace_tilde_in_str(token->lexeme, home);
		free(home);
	}
	else
		return ;
	token->type = WORD;
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
char	*peek_and_get_key(char *str)
{
	char	*end;

	if (!str)
		return (NULL);
	if (str && *(str + 1) != '$')
		return (NULL);
	end = str + 2;
	if (*end && ft_strchr("_a-zA-Z", *end))
		end++;
	else
		return (ft_strdup(""));
	while (*end && ft_strchr("_0-9a-zA-Z", *end))
		end++;
	return (ft_strndup(str + 2, (size_t)(end - (str + 2))));
}

/*
In a token lexeme replaces all $-indicated variables
with their respective values
or with empty string if not existing.
Returns the new lexeme, which must be freed.
*/
char	*replace_dollar_vars(t_darray *env_arr, char *lexeme)
{
	int		i;
	char	*new_lexeme;
	char	*key_value;
	char	*key;
	char	*temp;
	char	*temp2;
	char	*temp3;

	i = 0;
	new_lexeme = ft_strdup(lexeme);
	key = NULL;
	while (new_lexeme && new_lexeme[i])
	{
		key = peek_and_get_key(new_lexeme + i);
		if (key != NULL)
		{
			if (ft_strlen(key) == 0)
				key_value = NULL;
			else
				key_value = mini_get_env(env_arr, key + 1);
			temp = ft_strndup(new_lexeme, i + 1);
			temp2 = ft_strdup(new_lexeme + i + ft_strlen(key) + 2);
			temp3 = new_lexeme;
			new_lexeme = ft_strjoin3(temp, key_value, temp2);
			i = i + ft_strlen(key_value);
			free(temp);
			free(temp2);
			free(temp3);
			free(key);
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
	token->type = WORD;
	if (token->lexeme && ft_strchr(token->lexeme, '$'))
	{
		var = replace_dollar_vars(env_arr, token->lexeme);
		free(token->lexeme);
		token->lexeme = var;
	}
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

void	read_exit_status(t_data *data, t_token *token)
{
	char	*temp;

	debug("read_exit_status");
	temp = ft_itoa(data->exit_status);
	if (token)
	{
		free(token->lexeme);
		token->lexeme = temp;
		token->type = WORD;
	}
}

void	extract_string(t_token *token)
{
	char	*lexeme;

	debug("extract_string");
	if (token)
	{
		lexeme = ft_strtrim(token->lexeme, "'"); 
		free(token->lexeme);
		token->lexeme = lexeme;
		token->type = WORD;
	}
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

void	expand_string(t_data *data, t_token *token)
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
		if (get_token_type(string_tokens) == VAR_EXPANSION)
			expand_dollar(data->env_arr, get_curr_token(string_tokens));
		else if (get_token_type(string_tokens) == DOLLAR_QUESTION)
			read_exit_status(data, get_curr_token(string_tokens));
		temp_lexeme = token->lexeme;
		token->lexeme = ft_strjoin(temp_lexeme, ((t_token *)string_tokens->content)->lexeme);
		free(temp_lexeme);
		string_tokens = string_tokens->next;
	}
	ft_lstclear(&ptr_token_list, free_tokennode); //free_tokennode function is from scanner.h
	token->type = WORD;
	debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

/*
the function of the analyser is to walk on the tree and analyze and expand 
the nodes that need to.
Required is expansion of env variables (incl $PATH), of local variables and of $?
Optional are the ~, $!, etc..)
" " ' ' 
Also optional appear to be $(..) ${..} $'..' $".."
I replicate here the code for the print_ast function which is in parser.c
and walks the tree. The idea is to walk through each node... expand
and then pass it to the executor
*/
void analyse_expand(t_ast_node *ast, t_data *data)
{
	t_list	*tokenlist;

	if (ast == NULL)
		return ;
	tokenlist = ast->token_list;
	debug("analyse expand");
	which_ast_node(ast);
	while (tokenlist)
	{
		debug("Token type: %d ast node type: %d lexeme: %s", get_token_type(tokenlist), ast->type, get_token_lexeme(tokenlist));
		// if export do ~ expansion according to rules (expansion after '=' and ':' if valid path) for all tokens in tokenlist
		// if echo do ~ expansion if valid path
		if (get_token_type(tokenlist) == S_QUOTED_STRING)
			extract_string(get_curr_token(tokenlist));
		else if (get_token_type(tokenlist) == QUOTED_STRING)
			expand_string(data, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist) == TILDE || get_token_type(tokenlist) == PATHNAME)
			expand_path(data->env_arr, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist) == VAR_EXPANSION)
			expand_dollar(data->env_arr, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist)  == DOLLAR_QUESTION)
			read_exit_status(data, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist) == GLOBBING)
			expand_globbing(tokenlist);
		else
			debug("Token type not expanded");
		tokenlist = tokenlist->next;
	}
	debug("---------left -----------");
	if (ast->left)
		analyse_expand(ast->left, data);
	else
		debug("left is NULL");
	debug("---------right -----------");
	if (ast->right)
		analyse_expand(ast->right, data);
	else
		debug("right is NULL");
}
