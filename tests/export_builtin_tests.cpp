#include "razorclam_tests.h"
#include "../include/minishell.h"
#include <libft.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n);

const char *test_export()
{
	int			status;
	t_darray	*env_arr;
	t_list		*tokenlist;

	env_arr = NULL;
	if (!init_env_darray(&env_arr))
		return ("error in init env array");
	tokenlist = tokenizer("export VAR4=string VAR1 VAR2= VAR3=");
	status = execute_export_builtin(env_arr, tokenlist);
	my_assert(mini_get_env(env_arr, "VAR1") == NULL, "'export VAR1' falsely added VAR1 to env array");
	debug("VAR4 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR4"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR4"), "string", 7) == 0, "'export VAR4=string' did not correctly add 'string' string to VAR4");
	debug("VAR2 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR2"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR2"), "", 1) == 0, "'export VAR2=' did not add empty string value to VAR2");
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR3"), "", 1) == 0, "'export VAR3=' did not add empty string value to VAR3");
//	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR5"), "string", 7) == 0, "'export VAR5=\"string\"' did not correctly add 'string' string to VAR5");
//	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR6"), "2=3", 4) == 0, "'export VAR6=2=3' did not correctly add '2=3' string to VAR5");
	my_assert(status == 0, "export return status fail");
	return (NULL);
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();

	// run the tests
	run_test(test_export);

	return (NULL);
}

// works as a main
RUN_TESTS(all_tests);




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