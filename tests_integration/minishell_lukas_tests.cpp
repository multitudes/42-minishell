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

// since we will unset it better save it
const char* home = getenv("HOME");
std::string homeString = std::string(home);

/*
cd ../../../../../.. && pwd
*/
const char* test_cd() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd ../../../../../.. && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "/\n", "output is not root\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
cd ~ && pwd
*/
const char* test_cd2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd ~ && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);
	const char* home = std::getenv("HOME");
	debug("HOME -%s-", home);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert((strcmp(home, result.str().c_str())), "output is not the home dir\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
cd && pwd
*/
const char* test_cd3() 
{
    fflush(stdout);

    std::ostringstream result;

	// char *cwd = getcwd();
	std::string arg = "cd && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);
	const char* home = std::getenv("HOME");
	debug("HOME -%s-", home);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert((strcmp(home, result.str().c_str())), "output is not the home dir\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
cd ~/Desktop
*/
const char* test_cd4() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd ~/Desktop && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "/home/runner/Desktop\n" || result.str() == "/home/lbrusa/Desktop\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
unset HOME - cd ~ // should output home still
*/
const char* test_cd5() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset HOME && cd ~";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	arg = "unset HOME && cd ~ && pwd";
	exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(strcmp(home, result.str().c_str()), "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}

/*
export HOME=/Users/user42 && cd ~ // exit status is 1 because dir not found
*/
const char* test_cd6() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export HOME=/Users/user42 && cd ~";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd6\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
cd .. cool swag  // exit status 1
*/
const char* test_cd7() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd .. cool swag";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd6\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
cd Eyooooore // output is empty and exit status is 1
*/
const char* test_cd8() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd Eyooooore";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd6\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}


/*
again
cd -  
*/
const char* test_cd9() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd && cd / && cd - && pwd";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert((strcmp(home, result.str().c_str())), "output is not the home dir\n");
	my_assert(exit_status == 0, "exit status is not 1\n");
	return NULL;
}


/*
Here I get errors but no std out and exit status is 0
mkdir a - mkdir a/b - cd a/b - rm -r ../../a - cd ..
*/
const char* test_cd10() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "mkdir aa && mkdir aa/b && cd aa/b && rm -r ../../aa && cd ..";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd10\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


/*
because home is changed to "" the cd ~ should not change anything
"export HOME=""""
cd ~"
*/
const char* test_cd11() 
{
    fflush(stdout);

	// the current dir
	const char *curr_dir = getenv("PWD");

    std::ostringstream result;
	std::string arg = "export HOME=\"\" && cd ~";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd10\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	arg = "export HOME=\"\" && cd ~ && pwd";
	exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(strcmp(result.str().c_str(), curr_dir), "output is not correct cd10\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_echo() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "\n", "output is not correct echo\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_echo2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo $dontknow";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "\n", "output is not correct echo\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_echo3() 
{
    fflush(stdout);

	const char *path_c = getenv("PATH");
	if (path_c == nullptr) 
    	return "PATH is not set";
	std::string path(path_c);
	path = path + "\n";

    std::ostringstream result;
	std::string arg = "echo $PATH";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == path, "output is not correct echo3\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo ~
*/
const char* test_echo4() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "echo ~";
	uint8_t exit_status = run_command_and_check_output(arg, result);
	homeString = homeString + "\n";
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == homeString, "output is not correct echo4\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_echo5() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "echo -n -n -nnnn -nnnnm";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "-nnnnm", "output is not correct echo5\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo -n -nnn hello -n
*/
const char* test_echo6() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "echo -n -nnn hello -n";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello -n", "output is not correct echo6\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo "front$HOMEback"
*/
const char* test_echo7() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo \"front$HOMEback\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "front\n", "output is not correct echo7\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}



extern char **environ;

std::string getEnvironmentVariableAsString() {
	std::stringstream envString;
	for (char **env = environ; *env; ++env) {
		envString << *env << "\n";
	}
	return envString.str();
}
/*
test value of env command output

*/
const char* test_env() 
{
    fflush(stdout);

    std::string envVariables = getEnvironmentVariableAsString();

    std::ostringstream result;
	std::string arg = "env";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == envVariables, "output is not correct env\n");
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
	run_test(test_cd3);
	// run_test(test_cd4); cd ~/Desktop does not work 
	run_test(test_cd5);
	run_test(test_cd6);
	run_test(test_cd7);
	run_test(test_cd8);
	run_test(test_cd9);
	run_test(test_cd10);
	run_test(test_cd11);

	run_test(test_echo);
	run_test(test_echo2);
	run_test(test_echo3);
	run_test(test_echo4);
	run_test(test_echo5);
	run_test(test_echo6);

	run_test(test_env);


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
        return 1; // Error code for pclose failure
    } else {
        if (WIFEXITED(status)) {
            uint8_t exit_status = WEXITSTATUS(status);
            printf("Exit status: %d\n", exit_status);
            return exit_status; // Return the extracted exit status
        } else {
            printf("Command did not terminate normally\n");
            return 1; // Error code for abnormal termination
        }
    }
}
