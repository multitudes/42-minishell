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
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#include <cstdint> 

uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result);

// check if I am on GH actions and if not do not execute locally to save time
bool isRunningOnGitHubActions();
void createFileIfNotExists(const char* filename, const char* content);

// since we will unset the env variable it better save it first
const char* home = getenv("HOME");

const char* test_and() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("a", "ls -la");

    std::ostringstream result;
	std::string arg = "cat <a && echo e";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "ls -lae\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_and2() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("a", "ls -la");

    std::ostringstream result;
	std::string arg = "(cat <a && echo e) | echo hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_and3() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("a", "hello");

    std::ostringstream result;
	std::string arg = "cat <a";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_redir4() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("a", "ls -la");
	createFileIfNotExists("ab", "hello");

    std::ostringstream result;
	std::string arg = "cat ab > a && cat a";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_redir5() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("a", "ls -la");
	createFileIfNotExists("ab", "hello");
	createFileIfNotExists("ab2", "dont care");
    std::ostringstream result;
	std::string arg = "cat ab > ab2 > a && cat a";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_redir6() 
{
    fflush(stdout);

	// make sure thet the file is created if not exists
	createFileIfNotExists("ab2", "dont care");

	if (std::remove("ab2") != 0) {
		perror("Error deleting file");
	}
    std::ostringstream result;
	std::string arg = "cat < ab2";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}


const char* test_redir7() 
{
    fflush(stdout);

	createFileIfNotExists("a", "ls -la");
    
	std::ostringstream result;
	std::string arg = "cat a > b > c && cat c";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from splash: -%s-\n", result.str().c_str());
	my_assert(result.str() == "ls -la\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_and);
	run_test(test_and2);
	run_test(test_and3);
	run_test(test_redir4);
	run_test(test_redir5);
	run_test(test_redir6);
	

	return NULL;
}

RUN_TESTS(all_tests);


/**
 * Util function 
 */
bool isRunningOnGitHubActions() 
{
	const char* github_actions = std::getenv("GITHUB_ACTIONS");
	return github_actions != NULL && strcmp(github_actions, "true") == 0;
}


/**
 * util function to read from minishell using the popen call 
 * and the single command mode on the shell
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
            return exit_status; // Return the extracted exit status
        } else {
            fprintf(stderr, "Command did not terminate normally\n");
            return 1;
        }
    }
}


/**
 * Util function
 */
void createFileIfNotExists(const char* filename, const char* content) {
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open the file: " << filename << std::endl;
	} else {
		file << content;
	}

	file.close();
}

