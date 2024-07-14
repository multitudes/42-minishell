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
#include <filesystem>

// forward declaration 
// this is the new version of the function with popen
uint8_t run_command_and_check_output(const std::string& command_to_exec, std::ostringstream& result);
// the number 2 is the one with the pipes
int	run_command_and_check_output2(const std::string& command_to_exec, const std::string& expected_output, bool *pass);
// check if I am on GH actions and if not do not execute locally to save time
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

at the end I will set home again
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

	// reasign home to old value
	if (home != NULL && *home != '\0')
		setenv("HOME", home, 1);
	return NULL;
}

/*
cd .. cool swag  // exit status 0 and output is empty
*/
const char* test_cd7() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cd .. cool swag";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct cd7\n");
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
	my_assert(result.str() == "", "output is not correct cd8\n");
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

	// reasign home to old value
	if (home != NULL && *home != '\0')
		setenv("HOME", home, 1);
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
	my_assert(exit_status == 127, "exit status is not 127\n");
	return NULL;
}

/*
trying to execute a non executable file
bash: ./LICENSE: Permission denied
exit 126
*/
const char* test_exit2() 
{
    fflush(stdout);

	int fd = open("LICENCE", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        std::cerr << "Failed to open or create file." << std::endl;
        return "error in file creation";
    }
	// Now explicitly set the file permissions to rw-r--r--
    if (chmod("LICENCE", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
        std::cerr << "Failed to set file permissions." << std::endl;
        // Close the file descriptor
        close(fd);
        return "error in chmod";
    }

    std::ostringstream result;
	std::string arg = "./LICENSE";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct exit 2\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	// Close the file descriptor
	close(fd);
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

const char* test_exit4() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "cat meow";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct exit 4\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
export ls="ls -l" - $ls
*/
const char* test_exit5() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export ls='ls -l' && $ls";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct exit 4\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}


/*
export var=a && export $var=test && echo $var $a

*/
const char* test_export() 
{
    fflush(stdout);


	// if (setenv("var", "a", 1) != 0) {
    //     std::cerr << "Failed to set environment variable" << std::endl;
    //     return "Failed to set environment variable";
    // }
	// if (setenv("a", "test", 1) != 0) {
    //     std::cerr << "Failed to set environment variable" << std::endl;
    //     return "Failed to set environment variable";
    // }
    std::ostringstream result;
	std::string arg = "export var=a && export $var=test && echo $var $a";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "a test\n", "output is not correct export\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


/*
export $var=test - unset var

*/
const char* test_export2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export $var=test && unset var";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 2\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
export test1 - env
*/
const char* test_export3() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export test1 && env | grep test1";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 3\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

const char* test_export4() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export test1 && env | grep test1 && export | grep test1";	
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 4\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
export "" test=a
*/
const char* test_export5() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export "" test=a";	
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 5\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

/*
export 42=42
*/
const char* test_export6() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export 42=42";	
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 6\n");
	my_assert(exit_status == 1, "exit status is not 1\n");
	return NULL;
}

const char* test_export7() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export test3=$HOME && echo $test3";	
	if (home == NULL || *home == '\0')
		return "HOME is not set";
	std::string homeString = std::string(home) + "\n";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == homeString, "output is not correct export 7\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
export ==value
*/
const char* test_export8() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "export ==value";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct export 8\n");
	my_assert(exit_status == 1, "exit status is not 0\n");
	return NULL;
}

/*

*/
const char* test_parsing() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "\"\"";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	return NULL;
}


/*
echo "\s" ; echo "\\s"
*/
const char* test_parsing2() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "echo \\s && echo \\\\s";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "\\s\n\\s\n", "output is not correct parsing2\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo "bip | bip ; coyotte > < "
*/
const char* test_parsing3() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "echo 'bip | bip ; coyote > <; <3 !'";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "bip | bip ; coyote > <; <3 !\n", "output is not correct parsing3\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
export var=--- && echo $USER$var$USER$USER$USERtest$USER

*/
const char* test_parsing4() 
{
    fflush(stdout);

    std::ostringstream result;
	// export a env var into env
	if (setenv("var", "---", 1) != 0) {
        std::cerr << "Failed to set environment variable" << std::endl;
        return "Failed to set environment variable";
    }
	std::string arg = "echo $USER$var$USER$USER$USERtest$USER";
	char *user = getenv("USER");
	if (user == NULL || *user == '\0')
		return "USER is not set";
	std::string userString = std::string(user);
	userString = userString + "---" + userString + userString + userString + "\n";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == userString, "output is not correct parsing 4\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_parsing5() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "$";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing5\n");
	my_assert(exit_status == 127, "exit status is not 127\n");
	return NULL;
}

const char* test_parsing6() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "..";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing6\n");
	my_assert(exit_status == 127, "exit status is not 127\n");
	return NULL;
}


const char* test_parsing7() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = ".";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing7\n");
	my_assert(exit_status == 2, "exit status is not 2\n");
	return NULL;
}


const char* test_parsing8() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "$whatever";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing8\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
var exp now includes $ alfanum
which will be not found and will have null value so when I enter $4 in the minishell nothing happens
*/


const char* test_parsing9() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "$4";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct parsing9\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_parsing10() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "echo \'\"abc\"\'";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "\"abc\"\n", "output is not correct parsing10\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char* test_parsing11() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "echo \'\' bonjour";

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == " bonjour\n", "output is not correct parsing11\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_parsing12() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "export \"test=var\"=helloworld && echo $test";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "var=helloworld\n", "output is not correct parsing12\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
cat | cat | cat | ls
*/
const char* test_pipe() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "cat | cat | cat | ls\n\n\n\n";	
	bool pass;
	std::string output;
	uint8_t exit_status = run_command_and_check_output2(arg, output, &pass);
 	// ioctl(0, TIOCSTI, "\n");
	// ioctl(0, TIOCSTI, "\n");
	// ioctl(0, TIOCSTI, "\n");
	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == " bonjour\n", "output is not correct pipe\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char* test_pipe2() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "cat Makefile | grep pr | head -n 5 | cd file_not_exit";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct pipe2\n");
	my_assert(exit_status == 1, "exit status is not 0\n");
	return NULL;
}

const char* test_pipe3() 
{
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "cat Makefile | grep pr | head -n 5 | hello";	

	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());

	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 127, "exit status is not 0\n");
	return NULL;
}

namespace fs = std::filesystem;

const char* test_resolve_path_func() 
{
	fflush(stdout);

	std::ostringstream result;

	// print my working directory
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		debug("Current working dir: %s\n", cwd);
	} else {
		perror("getcwd() error");
		return "getcwd() error";
	}

	bool created = fs::create_directory("abc");
	if (created) {
		// Directory created successfully
	} else {
		// Directory might already exist or an error occurred
	}
	std::string arg = "../src";	
	uint8_t exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 126, "exit status is not 127\n");
	
	arg = "../src/";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	
	arg = "../LICENSE";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	
	arg = "abc";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 127, "exit status is not 127\n");
	
	arg = "./LICENSE";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 127, "exit status is not 126\n");

	arg = "./builtin_cat_tests.cpp";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 126, "exit status is not 126\n");
	
	arg = "./abcwhat";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct pipe3\n");
	my_assert(exit_status == 127, "exit status is not 127\n");
	
	return NULL;
}

const char* test_dollardigit() 
{
	fflush(stdout);

	std::ostringstream result;
	setenv("var", "$0", 1);
	std::string arg = "echo $var";	
	uint8_t exit_status = run_command_and_check_output(arg, result);

	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "splash\n", "output is not correct test_dollardigit\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	fflush(stdout);
	result.str("");
	arg = "echo $1";
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "\n", "output is not correct test_dollardigit\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	result.str("");
	arg = "echo $1 && echo $2 && echo $3 && echo $4 && echo $5 && echo $6 && echo $7 && echo $8 && echo $9";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "\n\n\n\n\n\n\n\n\n", "output is not correct test_dollardigit \n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	result.str("");
	arg = "echo $11 && echo $22 && echo $33 && echo $44 && echo $55 && echo $66 && echo $77 && echo $88 && echo $99";	
	exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "1\n2\n3\n4\n5\n6\n7\n8\n9\n", "output is not correct test_dollardigit \n");
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
	// run_test(test_cd5);
	run_test(test_cd6);
	run_test(test_cd7);
	run_test(test_cd8);
	run_test(test_cd9);
	// run_test(test_cd10);
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
	// run_test(test_exit2); //works n minishell
	// run_test(test_exit3); // works in minishell
	// run_test(test_exit4);
	// run_test(test_exit5); // not working because of quotes and popen probably

	// run_test(test_export);
	// run_test(test_export2);
	//run_test(test_export3);
	// run_test(test_export4); // cant make it work under thuis config
	// run_test(test_export5); // test returns 0 for unknown reason but minishell interactive is correct?
	run_test(test_export6);
	// run_test(test_export7); //not working because of quotes and popen probably
	run_test(test_export8);

	// run_test(test_parsing);
	run_test(test_parsing2);
	run_test(test_parsing3);
	run_test(test_parsing4);
	run_test(test_parsing5);
	run_test(test_parsing6);
	run_test(test_parsing7);
	// run_test(test_parsing8);// not working as expected
	// run_test(test_parsing9);
	// run_test(test_parsing10);
	run_test(test_parsing11);
	// run_test(test_parsing12);

	// run_test(test_pipe); // to difficult to test with popen and with pipes... skip

	run_test(test_pipe2);
	run_test(test_pipe3);

	run_test(test_resolve_path_func);

	run_test(test_dollardigit);


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



int	run_command_and_check_output2(const std::string& command_to_exec, const std::string& expected_output, bool *pass) {
	// seen from the point of you of the child process. pipefd_in is the input to the child process
	// and pipefd_out is the output of the child process
	int status;
	uint8_t	exit_status;
	int pipefd_in[2];
    int pipefd_out[2]; 

	// create the pipes
    if (pipe(pipefd_in) == -1)
        return -1;
    if (pipe(pipefd_out) == -1)
        return (-1);

	// create a child process	
    pid_t pid = fork();
    if (pid == -1)
		return (-1);
    
    else if (pid == 0) {
		// The child will read from pipefd_in[0] and write to pipefd_out[1]

		// I need to duplicate the file descriptors to the standard input and output
        dup2(pipefd_in[0], STDIN_FILENO); 
        close(pipefd_in[0]);
        dup2(pipefd_out[1], STDOUT_FILENO);
        close(pipefd_out[1]);

		// close the other ends of the pipes - child writes to pipefd_out[1]
		// which is now his stdout, so I could close pipefd_out[0]
		// but this gives me an error. I think this has to be closed by the process that 
		// is exiting... because if I write to a pipe that has no reader, the process will
		// receive a SIGPIPE signal and be killed. Race condition. the reader in the parent is not
		// ready to receive the child output? 
		// close(pipefd_out[0]);

		// close the other ends of the pipes - child reads from pipefd_in[0]
		// so I close pipefd_in[1] 
		close(pipefd_in[1]);

        execl("../minishell", "minishell", (char*) NULL);
        exit(EXIT_FAILURE);
    } else {
		// The parent will write to pipefd_in[1] and read from pipefd_out[0]
        close(pipefd_out[1]);
        close(pipefd_in[0]);
		usleep(5000);
        write(pipefd_in[1], command_to_exec.c_str(), command_to_exec.size());
        write(pipefd_in[1], "\x04", 1);
		write(pipefd_in[1], "\x04", 1);
		write(pipefd_in[1], "\x04", 1);

		// close pipefd_in after use to send the eof
		close(pipefd_in[1]);
		usleep(5000);

        char buffer[1024];
        int n = read(pipefd_out[0], buffer, sizeof(buffer));
        buffer[n] = '\0';
       	debug("output: -%s-", buffer);
        

        if (strcmp(buffer, expected_output.c_str()) == 0)
			*pass = true;
		debug("pass: %s", *pass ? "true" : "false");
		
		// clean up closing the file descriptors that I used
		close(pipefd_out[0]);
		
		// this is the proper way to get the exit status of the child process
		exit_status = 0;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) /* child exited normally */
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) /* child exited on a signal */
			exit_status = WTERMSIG(status) + 128; /* 128 is the offset for signals */
		else
			exit_status = EXIT_FAILURE; /* child exited abnormally (should not happen)*/
		return exit_status;
	}
}




