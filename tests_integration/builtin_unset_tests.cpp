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
bool make_directory_read_only(const std::string& path);
bool isRunningOnGitHubActions();

const char* home = getenv("HOME");

const char* test_unset_read_only() {

	make_directory_read_only(".non_read_test_dir");

    fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset PPID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}



const char* test_unset_read_only_EUID() {
	
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset EUID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}

const char* test_unset_read_only_UID() {
	
	fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset UID=123";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}


/*
these tests will pass but exit with status 1 because the variable is read only

*/
const char* test_unset_read_only2() {
	make_directory_read_only(".non_read_test_dir");
	
	fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset PPID";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 0\n");


	return NULL;
}



const char* test_unset_read_only_EUID2() {
	fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset EUID";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 0\n");

	return NULL;
}

const char* test_unset_read_only_UID2() {
	
	fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset UID";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 0\n");

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
		run_test(test_unset_read_only);
		run_test(test_unset_read_only_EUID);
		run_test(test_unset_read_only_UID);
		run_test(test_unset_read_only2);
		run_test(test_unset_read_only_EUID2);
		run_test(test_unset_read_only_UID2);
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

bool make_directory_read_only(const std::string& path) {
    // Check if directory already exists
    if (access(path.c_str(), F_OK) == -1) {
        // If not, create it
        if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
            // handle error, for example print the error message to stderr
            perror("mkdir");
            return false;
        }
    }

    // Change permissions to read-only
    if (chmod(path.c_str(), S_IRUSR | S_IRGRP | S_IROTH) == -1) {
        // handle error, for example print the error message to stderr
        perror("chmod");
        return false;
    }

    return true;
}

bool isRunningOnGitHubActions() 
{
	// const char* github_actions = std::getenv("GITHUB_ACTIONS");
	// return github_actions != NULL && strcmp(github_actions, "true") == 0;
	return true;
}
