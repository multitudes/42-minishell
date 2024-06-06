#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/globbing.h"

// needed for the tests
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

extern char **environ;


/*
REMEMBER to run valgrind to check for memory leaks ex:
valgrind --leak-check=full ./unset_builtin_tests
*/
const char *test_unset()
{
	t_darray *env_arr = NULL;
	int result = 0;
	t_list *tokenlist;
	char* position = NULL;

	if (!init_env_darray(&env_arr))
		return "error in init env array";
	position = mini_get_env(env_arr, "PWD");
	my_assert(position != NULL, "PWD not set");
	tokenlist = tokenizer("unset PWD");
	result = execute_unset_builtin(env_arr, tokenlist);
	my_assert(result == 0, "unset PWD fail");
	position = mini_get_env(env_arr, "PWD");
	my_assert(position == NULL, "PWD not unset")
	update_env(env_arr, "VAR", "=4567");
	position = mini_get_env(env_arr, "VAR");
	my_assert(ft_strncmp(position, "=4567", 6) == 0, "VAR==4567 not correctly added to env");
	tokenlist = tokenizer("unset VAR=");
	result = execute_unset_builtin(env_arr, tokenlist);
	position = mini_get_env(env_arr, "VAR=");
	my_assert(ft_strncmp(position, "=4567", 6) == 0, "VAR==4567 incorrectly removed from env");
	my_assert(result == 0, "unset VAR= fail");
	tokenlist = tokenizer("unset VAR");
	result = execute_unset_builtin(env_arr, tokenlist);
	position = mini_get_env(env_arr, "VAR");
	my_assert(position == NULL, "VAR not correctly removed");
	my_assert(result == 0, "unset VAR fail");
	
	debug("result %d", result);

	// free tokens
	ft_lstclear(&tokenlist, free_tokennode);
	// free env darray
	darray_clear_destroy(env_arr);

	return NULL;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_unset);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);



//avoiding adding the whole libft only for this
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	**l;
	t_list	*temp;

	if (lst == NULL || *lst == NULL)
		return ;
	l = lst;
	temp = *lst;
	while ((*lst)->next)
	{
		*lst = (*lst)->next;
		ft_lstdelone(temp, del);
		temp = *lst;
	}
	ft_lstdelone(temp, del);
	*l = NULL;
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (s1 == NULL && s2 == NULL)
		return (0);
	if (s1 == NULL)
		return (-1);
	if (s2 == NULL)
		return (1);
	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}