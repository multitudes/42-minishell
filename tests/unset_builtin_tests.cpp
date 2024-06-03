#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/globbing.h"


const char *test_unset()
{
	//int		execute_unset_builtin(t_data *data, t_list *tokenlist);
	// int result = 0;
	
	// result = execute_unset_builtin(
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
