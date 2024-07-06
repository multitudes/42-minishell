#include "razorclam_tests.h"
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <array>
#include <sstream>

// forward declaration
uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result);
bool isRunningOnGitHubActions();


const char* test_cd() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd ../../../../../.. && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "/\n", "output is not hello\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_cd2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd ~ && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "/home/runner\n" || result.str() == "/home/lbrusa\n", "output is not hello\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_cd);
	run_test(test_cd2);

	return NULL;
}

RUN_TESTS(all_tests);

bool isRunningOnGitHubActions() 
{
	const char* github_actions = std::getenv("GITHUB_ACTIONS");
	return github_actions != NULL && strcmp(github_actions, "true") == 0;
}


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
