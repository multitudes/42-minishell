#include "razorclam_tests.h"
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <array>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

// forward declaration
uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result);
// int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass);
bool isRunningOnGitHubActions();


const char* test_export_read_only() 
{

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "export PPID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	
	return NULL;
}

const char* test_export_read_only_EUID() {
		fflush(stdout);

    std::ostringstream result;
	std::string arg = "export EUID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	
	return NULL;
}

const char* test_export_read_only_UID() {
	fflush(stdout);

    std::ostringstream result;
	std::string arg = "export UID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	
	return NULL;
}

/*
access righhts on files
*/
const char *all_tests()
{
	if (isRunningOnGitHubActions())
	{
		// necessary to start the test suite
		suite_start();
		// run the tests
		run_test(test_export_read_only);
		run_test(test_export_read_only_EUID);
		run_test(test_export_read_only_UID);
	}
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);


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
        return 1; // Error code for pclose failure
    } else {
        if (WIFEXITED(status)) {
            uint8_t exit_status = WEXITSTATUS(status);
            // printf("Exit status: %d\n", exit_status);
            return exit_status; // Return the extracted exit status
        } else {
            fprintf(stderr, "Command did not terminate normally\n");
            return 1; // Error code for abnormal termination
        }
    }
}

bool isRunningOnGitHubActions() 
{
	// const char* github_actions = std::getenv("GITHUB_ACTIONS");
	// return github_actions != NULL && strcmp(github_actions, "true") == 0;
	return true;
}
