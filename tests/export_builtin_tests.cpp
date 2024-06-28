#include "razorclam_tests.h"
#include "../include/minishell.h"
#include <libft.h>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/builtins.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n);

const char *test_export()
{
	int			status;
	t_darray	*env_arr;
	t_list		*tokenlist;

	env_arr = NULL;
	if (!init_env_darray(&env_arr))
		return ("error in init env array");
	tokenlist = tokenizer("export VAR1=first var1=second pwd= PWD=newPWD VAR5=2=3 VAR6");
	status = execute_export_builtin(env_arr, tokenlist);
	debug("VAR1 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR1"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR1"), "first", 6) == 0, "export VAR1=first did not correctly add value first to VAR1");
	debug("VAR2 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR2"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "var1"), "second", 7) == 0, "export var1=second did not correctly add value second to var1");
	debug("VAR3 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR3"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "pwd"), "", 1) == 0, "export pwd= did not add pwd and empty string value to pwd");
	debug("VAR4 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR4"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "PWD"), "newPWD", 7) == 0, "export PWD=newPWD did not correctly change value of PWD to newPWD");
	debug("VAR5 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR5"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR5"), "2=3", 4) == 0, "export VAR5=2=3 did not correctly add VAR5 and value 2=3");
	debug("VAR6 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "VAR6"));
	my_assert(mini_get_env(env_arr, "VAR6") == NULL, "'export VAR6' falsely added VAR6 to env array");
	my_assert(status == 0, "export return status fail");
	tokenlist = tokenizer("export ~VAR7=23");
	status = execute_export_builtin(env_arr, tokenlist);
	my_assert(status == 1, "export return status fail");
	debug("VAR7 >>>>>>>>>>>>>>> -%s-", mini_get_env(env_arr, "~VAR7"));
	my_assert(ft_strncmp(mini_get_env(env_arr, "VAR7"), "23", 3) != 0, "export ~VAR7=23 incorrectly added ~VAR7 to env");
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
