#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include <fstream>

const char* test_basicminishell() {
	
	return NULL;
}


const char *test_basicminishell2() {
    
	return NULL;
}

const char *test_basicminishell3() {
    
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_basicminishell);
	run_test(test_basicminishell2);
	run_test(test_basicminishell3);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);

