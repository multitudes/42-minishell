#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/globbing.h"

extern char **environ;

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
