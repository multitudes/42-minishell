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
exit hello
*/
const char* test_builtin() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 255, "exit status is not 255\n");
	return NULL;
}

/*
exit -"100"
*/
const char* test_builtin2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit -\"100\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 156, "exit status is not 255\n");
	return NULL;
}

/*
exit +"100"
*/
const char* test_builtin3() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit +\"100\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 100, "exit status is not 255\n");
	return NULL;
}

/*
exit "-100"
*/
const char* test_builtin4() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit \"-100\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 156, "exit status is not 255\n");
	return NULL;
}

/*
exit "+100"
*/
const char* test_builtin5() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit \"+100\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 100, "exit status is not 255\n");
	return NULL;
}

/*
exit 100 morestuff
*/
const char* test_builtin6() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit 100 toomany";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 1, "exit status is not 255\n");
	return NULL;
}

/*
exit  more stuff
*/
const char* test_builtin7() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "exit too many";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "exit\n", "output is not correct\n");
	my_assert(exit_status == 255, "exit status is not 255\n");
	return NULL;
}

/*
unset PATH 
check exit status

*/
const char* test_builtin_unset() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "unset PATH && echo $?";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "0\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 255\n");
	return NULL;
}


/*
echo "$"
*/
const char* test_builtin_echo() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo '$'";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "$\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo "$"
*/
const char* test_builtin_echo2() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo \"$\"";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "$\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

/*
echo a c b  | sort
*/
const char* test_builtin_echo3() 
{
    fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo a c b  | sort";
	uint8_t exit_status = run_command_and_check_output(arg, result);

    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "a c b\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_builtin);
	run_test(test_builtin2);
	run_test(test_builtin3);
	run_test(test_builtin4);
	run_test(test_builtin5);
	run_test(test_builtin6);
	run_test(test_builtin7);

	run_test(test_builtin_unset);

	run_test(test_builtin_echo);
	// run_test(test_builtin_echo2); 
	run_test(test_builtin_echo3);




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




