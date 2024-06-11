#include "razorclam_tests.h"
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include <string>
#include <cstring>
#include <array>
#include <sstream>

// forward declaration
bool isRunningOnGitHubActions();

/*
util function to read from minishell using the popen call and the single command mode
*/
uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result) 
{
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

	int status = pclose(fp);
	if (status == -1) {
		perror("pclose");
		return 1;
	} else {
		if (WIFEXITED(status)) {
			printf("Exit status: %d\n", WEXITSTATUS(status));
		} else {
			printf("Command did not terminate normally\n");
		}
	}
    debug("result: -%s- status %d", result.str().c_str(), status);

    return 0;
}


const char* test_popen() 
{

    debug("running test_popen\n");
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello\n", "output is not hello\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
more minishell tests.
test echo -n -nnn hello -n
*/
const char* test_echo() 
{

    debug("running test_popen\n");
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -n -nnn hello -n";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello ", "output is not hello\n");
	debug("exit_status: %d\n", exit_status);
	// my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
print the value of the HOME environment variable

*/
const char* test_echo2() 
{
    debug("running test_popen\n");
    fflush(stdout);
	// get the value of the HOME environment variable
	// I dont need to check if HOME is not set
	if (getenv("HOME") == NULL) {
		return NULL;
	}
	const char* home = getenv("HOME");
	debug("home: -%s-", home);
    if (home == NULL) {
        perror("getenv");
        return "getenv failed";
    }

    std::ostringstream result;
	std::string arg = "echo $HOME";
	uint8_t exit_status = run_command_and_check_output(arg, result);
	std::string output = result.str();
    // remove the trailing newline from the output
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    debug("output: -%s-", output.c_str());
	my_assert(exit_status == 0, "exit status is not 0\n");
	debug("home: -%s-", home);
	my_assert(output == home, "output is not as expected\n");
	return NULL;
}


/*
print the value of the HOME environment variable

*/
const char* test_echo3() 
{

    debug("running test_popen\n");
    fflush(stdout);

	uint8_t exit_status;

	// set the current dit to minishell
	if(setenv("VAR", "hello", 1) == -1) {
	    perror("setenv");
        return "setenv failed";
    }
    std::ostringstream result;
	std::string arg = "echo $VAR";
	exit_status = run_command_and_check_output(arg, result);
	
	std::string output = result.str();
    // remove the trailing newline from the output
    // if (!output.empty() && output.back() == '\n') {
    //     output.pop_back();
    // }
    debug("output: -%s-", output.c_str());
	my_assert(output == "hello\n", "output is not as expected\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_popen);
	//run_test(test_echo);
	run_test(test_echo2);
	run_test(test_echo3);

	return NULL;
}

RUN_TESTS(all_tests);

bool isRunningOnGitHubActions() 
{
	const char* github_actions = std::getenv("GITHUB_ACTIONS");
	return github_actions != NULL && strcmp(github_actions, "true") == 0;
}
