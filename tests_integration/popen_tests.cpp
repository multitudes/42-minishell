#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include <fstream>

#include <string>
#include <cstring>
#include <array>
#include <sstream>


/*
util function to read from minishell
*/
bool run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result) {
    debug("running test_popen\n");
    fflush(stdout);

    std::string command = "../minishell -c \"" + command_to_exec + "\"";
    FILE *fp = popen(command.c_str(), "r");
    if (fp == NULL) {
        perror("popen");
        return false;
    }

    std::array<char, 128> buffer;
    while (fgets(buffer.data(), buffer.size(), fp) != NULL) {
        result << buffer.data();
    }

    if (result.str() != "hello\n") {
        fprintf(stderr, "output is not hello\n");
        return false;
    }

    pclose(fp);
    debug("result: %s\n", result.str().c_str());

    return true;
}


const char* test_popen() {

    debug("running test_popen\n");
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo hello";
	if (!run_command_and_check_output(arg, result)) {
		return "test_popen failed";
	}
    debug("result: %s\n", result.str().c_str());
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_popen);


	return NULL;
}

RUN_TESTS(all_tests);