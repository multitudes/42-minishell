#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"


const char* test_basicminishell() {
	// I would like to test my minishell here

	return NULL;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_basicminishell);

	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);

