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


const char *test_basicminishell_echo() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}


const char *test_basicminishell_echo2() {


	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -n";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");


	return NULL;
}


const char *test_basicminishell_echo3() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -nnn -nn";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");


	return NULL;
}


const char *test_basicminishell_echo4() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -nnn -nn -mnnn hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "mnnn hello\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}


const char *test_basicminishell_echo5() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -mnnn hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "mnnn hello\n", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");


	return NULL;
}


const char *test_basicminishell_echo6() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo $NonExistingVar";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");


	return NULL;
}


const char *test_basicminishell_echo7() {
	
	if (getenv("HOME") != NULL)
	{

		fflush(stdout);

		std::ostringstream result;
		std::string arg = "echo $HOME";
		std::string my_home = getenv("HOME");
		uint8_t exit_status = run_command_and_check_output(arg, result);
		debug("result from minishell: -%s-\n", result.str().c_str());
		my_assert(result.str() == my_home, "output is not correct\n");
		my_assert(exit_status == 0, "exit status is not 0\n");
	}
	return NULL;
}

const char *test_basicminishell_echo8() {
		fflush(stdout);

		std::ostringstream result;
		std::string arg = "echo $HOMe";
		uint8_t exit_status = run_command_and_check_output(arg, result);
		debug("result from minishell: -%s-\n", result.str().c_str());
		my_assert(result.str() == "", "output is not correct\n");
		my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}

//	std::string command_to_exec = "echo ~\n";
const char *test_basicminishell_echo9() {

	if (getenv("HOME") != NULL)
	{

		fflush(stdout);

		std::ostringstream result;
		std::string arg = "echo ~";
		std::string my_home = getenv("HOME");
		uint8_t exit_status = run_command_and_check_output(arg, result);
		debug("result from minishell: -%s-\n", result.str().c_str());
		my_assert(result.str() == my_home, "output is not correct\n");
		my_assert(exit_status == 0, "exit status is not 0\n");
	}
	
	return NULL;
}

const char *test_basicminishell_echo10() {
	fflush(stdout);

	std::ostringstream result;
	std::string arg = "echo $PATH";
	std::string my_path = getenv("PATH");
	uint8_t exit_status = run_command_and_check_output(arg, result);
	debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == my_path, "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");

	return NULL;
}

const char *test_basicminishell_echo11() {

	fflush(stdout);

    std::ostringstream result;
	std::string arg = "echo -n hello";
	uint8_t exit_status = run_command_and_check_output(arg, result);
    debug("result from minishell: -%s-\n", result.str().c_str());
	my_assert(result.str() == "hello", "output is not correct\n");
	my_assert(exit_status == 0, "exit status is not 0\n");
	return NULL;
}


const char *all_tests()
{
	if (isRunningOnGitHubActions())
	{
		// necessary to start the test suite
		suite_start();
		
		// run the tests
		run_test(test_basicminishell_echo);
		run_test(test_basicminishell_echo2);
		run_test(test_basicminishell_echo3);
		run_test(test_basicminishell_echo4);
		run_test(test_basicminishell_echo5);
		run_test(test_basicminishell_echo6);
		run_test(test_basicminishell_echo7);
		run_test(test_basicminishell_echo8);
		run_test(test_basicminishell_echo9);
		run_test(test_basicminishell_echo10);
		run_test(test_basicminishell_echo11);

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

// int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass) {
// 	// seen from the point of you of the child process. pipefd_in is the input to the child process
// 	// and pipefd_out is the output of the child process
// 	int status;
// 	int	exit_status;
// 	int pipefd_in[2];
//     int pipefd_out[2]; 

// 	// create the pipes
//     if (pipe(pipefd_in) == -1)
//         return -1;
//     if (pipe(pipefd_out) == -1)
//         return (-1);

// 	// create a child process	
//     pid_t pid = fork();
//     if (pid == -1)
// 		return (-1);
    
//     else if (pid == 0) {
// 		// The child will read from pipefd_in[0] and write to pipefd_out[1]

// 		// I need to duplicate the file descriptors to the standard input and output
//         dup2(pipefd_in[0], STDIN_FILENO); 
//         close(pipefd_in[0]);
//         dup2(pipefd_out[1], STDOUT_FILENO);
//         close(pipefd_out[1]);

// 		// close the other ends of the pipes - child writes to pipefd_out[1]
// 		// which is now his stdout, so I could close pipefd_out[0]
// 		// but this gives me an error. I think this has to be closed by the process that 
// 		// is exiting... because if I write to a pipe that has no reader, the process will
// 		// receive a SIGPIPE signal and be killed. Race condition. the reader in the parent is not
// 		// ready to receive the child output? 
// 		// close(pipefd_out[0]);

// 		// close the other ends of the pipes - child reads from pipefd_in[0]
// 		// so I close pipefd_in[1] 
// 		close(pipefd_in[1]);

//         execl("../minishell", "minishell", (char*) NULL);
//         exit(EXIT_FAILURE);
//     } else {
// 		// The parent will write to pipefd_in[1] and read from pipefd_out[0]
//         close(pipefd_out[1]);
//         close(pipefd_in[0]);
		
// 		if (!isRunningOnGitHubActions())
// 			usleep(5000);
//         else 
//             usleep(2000);

//         write(pipefd_in[1], command_to_exec.c_str(), command_to_exec.size());
//         // write(pipefd_in[1], "\x04", 1);

// 		// close pipefd_in after use to send the eof
// 		close(pipefd_in[1]);
		
//         if (!isRunningOnGitHubActions())
//             usleep(5000);
//         else 
//             usleep(2000);
//         char buffer[1024];
//         int n = read(pipefd_out[0], buffer, sizeof(buffer));
//         buffer[n] = '\0';
//        	debug("output: -%s-", buffer);
        

//         if (strcmp(buffer, expected_output.c_str()) == 0)
// 			*pass = true;
// 		debug("pass: %s", *pass ? "true" : "false");
		
// 		// clean up closing the file descriptors that I used
// 		close(pipefd_out[0]);
		
// 		// this is the proper way to get the exit status of the child process
// 		exit_status = 0;
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status)) /* child exited normally */
// 			exit_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status)) /* child exited on a signal */
// 			exit_status = WTERMSIG(status) + 128; /* 128 is the offset for signals */
// 		else
// 			exit_status = EXIT_FAILURE; /* child exited abnormally (should not happen)*/
// 		return exit_status;
// 	}
// }


bool isRunningOnGitHubActions() 
{
    const char* githubActions = std::getenv("GITHUB_ACTIONS");
    return githubActions != nullptr && githubActions == std::string("true");
}
