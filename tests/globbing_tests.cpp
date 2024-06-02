#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/globbing.h"


const char *test_globbing_get_files()
{
	// int result = 0;
	// char **files = NULL;
	// result = get_files_in_directory(files);
	// debug("result: %d", result);
	// debug("files: %s", *files); 
	// my_assert(result == 0, "get_files_in_directory failed");
	return NULL;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_globbing_get_files);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
