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

// forward declaration
uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result);
bool isRunningOnGitHubActions();

// since we will unset it better save it
const char* home = getenv("HOME");

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
	if (home == NULL || *home == '\0')
		return "HOME is not set";
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
	
	if (home == NULL || *home == '\0')
		return "HOME is not set";
	std::string homeString = std::string(home);
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

	// std::cout << "-----------------------------------" << std::endl;
	// std::cout << result.str() << std::endl;
	// std::cout << "-----------------------------------" << std::endl;
	// std::cout << envVariables << std::endl;
	// std::cout << "-----------------------------------" << std::endl;
    debug("result from minishell: -%s-\n", result.str().c_str());
	debug("envVariables: -%s-\n", envVariables.c_str());
	my_assert(result.str() == envVariables, "output is not correct env\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
Do export test1 && export test2=hi <- test1 should not show in env

cnnot make this test work with popen? works when calling minishell
*/
const char* test_env2() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "export test1 && export test2=hi && env | grep test";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "_=export test1 && export test2=hi && env | grep test\ntest2=hi\n", "output is not correct env2\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_env3() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "env what?";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct env2\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
exit status
*/
const char* test_exit() 
{
    fflush(stdout);


    std::ostringstream result;
	std::string arg = "what?";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct env2\n");
	my_assert(exit_status == 127, "exit status is not 1\n");
	return NULL;
}

/*
trying to execute a non executable file

*/
const char* test_exit2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "./LICENSE";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct exit 2\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	return NULL;
}

const char* test_exit3() 
{
    fflush(stdout);

	// modify the permissions of the file!
	const char* filename = "../LICENSE"; // Path to the file
    struct stat st;
    if (stat(filename, &st) == 0) {
        // Add execute permissions for user, group, and others
        mode_t mode = st.st_mode | S_IXUSR | S_IXGRP | S_IXOTH;
        if (chmod(filename, mode) == -1) {
            perror("chmod failed");
            return "chmod failed";
        }
    } else {
        perror("stat failed");
        return "stat failed";
    }


    std::ostringstream result;
	std::string arg = "./LICENSE";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct exit 2\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
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

	// run_test(test_env);
	// run_test(test_env2); // cannot make this work with popen
	run_test(test_env3);

	run_test(test_exit);
	run_test(test_exit2);
	run_test(test_exit3);


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
            // printf("Exit status: %d\n", exit_status);
            return exit_status; // Return the extracted exit status
        } else {
            fprintf(stderr, "Command did not terminate normally\n");
            return 1; // Error code for abnormal termination
        }
    }
}
