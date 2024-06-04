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