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

char	*replace_tilde_in_str(char *str, char *home)
{
	int		i;
	int		j;
	int		k;
	char	*exp_str;
	int		tilde_count;


	i = 0;
	j = 0;
	k = 0;
	exp_str = NULL;
	tilde_count = count_chars_in_str(str, '~');e
	debug("replace tilde in str, counted '~': %i", tilde_count);
	exp_str = ft_calloc(ft_strlen(str) + tilde_count * ft_strlen(home) - tilde_count + 1, sizeof(char));
	if (!exp_str)
		return (null_on_err("malloc"));
	while (str && str[i])
	{
		if (str[i] == '~')
		{
			while (home && home[j])
			{
				exp_str[k + j] = home[j];
				j++;
			}
			k = k + j - 1;
			j = 0;
		}
		else
			exp_str[k] = str[i];
		i++;
		k++;
	}
	return (exp_str);
}

/*
Get home from environment or if HOME is unset,
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
		char	*temp;
		char	*home;

		debug("expand_path");
		lexeme = NULL;
		if (token)
			lexeme = token->lexeme;
		home = get_home(env_arr);
		if (token && token->type == TILDE)
			temp = home;
		else if (token && token->type == PATHNAME)
			temp = replace_tilde_in_str(lexeme, home);
		else
			temp = NULL;
		free(lexeme);
		token->lexeme = temp;
		token->type = WORD;
		debug("Expanded token: %s, type: %i", token->lexeme, token->type);
}

/*
Used to expand variables, indicated by "$".
*/
void	expand_variable(t_darray *env_arr, t_token *token)
{
		char	*var;
		char	*temp;

		debug("expand_variable");
		if (token)
			var = token->lexeme;
		else
			return ;
		if (mini_get_env(env_arr, var + 1))
			temp = ft_strdup(mini_get_env(env_arr, var + 1));
		else
			temp = NULL;
		free(var);
		token->lexeme = temp;
		token->type = WORD;
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
	debug("extract_string");
	if (token)
	{
		ft_strlcpy(token->lexeme, (const char *)token->lexeme + 1, ft_strlen(token->lexeme) - 1);
		token->type = WORD;
	}
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
	unquoted_lexeme = ft_strtrim(token->lexeme, "\""); // check, is this behaves well, when we have strings like "\"djklfjsdl\"" or ""dkldfj";
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
			expand_variable(data->env_arr, get_curr_token(string_tokens));
//		if (get_token_type(string_tokens) == TILDE)
//			expand_variable(data->env_arr, get_curr_token(string_tokens));
		else if (get_token_type(string_tokens) == DOLLAR_QUESTION)
			read_exit_status(data, get_curr_token(string_tokens));
		temp_lexeme = token->lexeme;
		token->lexeme = ft_strjoin(temp_lexeme, ((t_token *)string_tokens->content)->lexeme);
		free(temp_lexeme);
		string_tokens = string_tokens->next;
	}
	ft_lstclear(&ptr_token_list, free_tokennode); //free_tokennode function is from scanner.h
	token->type = WORD;
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
		if (get_token_type(tokenlist) == TILDE || get_token_type(tokenlist) == PATHNAME)
			expand_path(data->env_arr, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist) == VAR_EXPANSION)
			expand_variable(data->env_arr, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist)  == DOLLAR_QUESTION)
			read_exit_status(data, get_curr_token(tokenlist));
		else if (get_token_type(tokenlist)  == S_QUOTED_STRING)
			extract_string(get_curr_token(tokenlist));
		else if (get_token_type(tokenlist)  == QUOTED_STRING)
			expand_string(data, get_curr_token(tokenlist));
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
