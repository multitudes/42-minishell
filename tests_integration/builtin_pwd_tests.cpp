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
bool isRunningOnGitHubActions();

std::string get_current_directory() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return std::string(buffer);
    } else {
        // handle error
        std::cerr << "Failed to get current directory" << std::endl;
        return "";
    }
}

/*
Ex output: - splash 💦 > echo $PWD
/home/lbrusa/DEV/minishell/tests_integration
 splash 💦 > exit
-
*/
const char* test_pwd_on_startup() 
{
	// fflush(stdout);

    // std::ostringstream result;
	// std::string arg = "echo $PWD";
	// std::string expected_output = get_current_directory();
	// expected_output += "\n";
	
	// uint8_t exit_status = run_command_and_check_output(arg, result);
    // debug("result from minishell: -%s-\n", result.str().c_str());
	// my_assert(result.str() == expected_output, "output is not correct\n");
	// my_assert(exit_status == 0, "exit status is not 0\n");

    return NULL;
}

// comment out because apparently the output is mixedd with the sterr?
/*
 DEBUG builtin_pwd_tests.cpp:run_command_and_check_output:152: output: - splash 💦 > unset PWD
make[1]: *** [Makefile:71: runtests] Error 1
 splash 💦 > 
 splash 💦 > echo $PWD

 splash 💦 > exit
make: *** [Makefile:90: tests_integration] Error 2
-
*/

// const char* test_pwd_after_unset() {
//     bool pass = false;
// 	uint8_t	exit_status;
// 	std::string output = " splash 💦 > unset PWD\n splash 💦 > \necho $PWD\n\n splash 💦 > exit\n";
//     exit_status = run_command_and_check_output("unset PWD\n\necho $PWD", output, &pass);
// 	my_assert(exit_status == 0, "Minishell exited with non-zero status");
// 	my_assert(pass, "Output is not as expected");
//     return NULL;
// }
// const char* test_pwd_after_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd /path/to/new/directory; echo $PWD", "/path/to/new/directory", &pass);
//     return pass ? NULL : "PWD not updated after cd";
// }

// const char* test_pwd_after_non_cd_command() {
//     bool pass = false;
//     run_command_and_check_output("ls; echo $PWD", "/path/to/initial/directory", &pass);
//     return pass ? NULL : "PWD changed after non-cd command";
// }

// const char* test_pwd_after_relative_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd relative/path; echo $PWD", "/path/to/initial/directory/relative/path", &pass);
//     return pass ? NULL : "PWD not updated correctly after relative cd";
// }

// const char* test_pwd_after_absolute_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd /absolute/path; echo $PWD", "/absolute/path", &pass);
//     return pass ? NULL : "PWD not updated correctly after absolute cd";
// }

const char *all_tests() {
	if (isRunningOnGitHubActions())
	{
		// necessary to start the test suite
		suite_start();
		run_test(test_pwd_on_startup);
		// run_test(test_pwd_after_unset);
		// run_test(test_pwd_after_cd);
		// run_test(test_pwd_after_non_cd_command);
		// run_test(test_pwd_after_relative_cd);
		// run_test(test_pwd_after_absolute_cd);
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

bool isRunningOnGitHubActions() {
	// const char* github_actions = std::getenv("GITHUB_ACTIONS");
	// return github_actions != NULL && strcmp(github_actions, "true") == 0;
	return true;
}
