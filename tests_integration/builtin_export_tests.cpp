#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include <fstream>

#include <string>
#include <cstring>

// forward declaration
int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass);
bool isRunningOnGitHubActions();

/*
bool    read_only_variable(const char *key)
{
    if (ft_strncmp(key, "PPID", 5) == 0)
        return (true);
    else if (ft_strncmp(key, "EUID", 5) == 0)
        return (true);
    else if (ft_strncmp(key, "UID", 4) == 0)
        return (true);
    else
        return (false);
}
*/
const char* test_export_read_only() {
	/*
	bool pass = false;

	// test export read only
	int exit_status = run_command_and_check_output("export PPID=123\n", \
	"minishell $ export PPID=123\nminishell $ exit\n", &pass);
	my_assert(pass, "pass is not as expected");
	my_assert(exit_status == 2, "exit status is not as expected");
	*/
	
	return NULL;
}

const char* test_export_read_only_EUID() 
{
/*
	bool pass = false;

	// test export read only
	int exit_status = run_command_and_check_output("export EUID=123\n", \
	"minishell $ export EUID=123\nminishell $ exit\n", &pass);
	my_assert(pass, "pass is not as expected");
	my_assert(exit_status == 2, "exit status is not as expected");
*/
	
	return NULL;
}

const char* test_export_read_only_UID() 
{
	/*
	bool pass = false;

	// test export read only
	int exit_status = run_command_and_check_output("export UID=123\n", \
	"minishell $ export UID=123\nminishell $ exit\n", &pass);
	my_assert(pass, "pass is not as expected");
	my_assert(exit_status == 2, "exit status is not as expected");
	*/
	
	return NULL;
}

/*
access righhts on files
*/
const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_export_read_only);
	run_test(test_export_read_only_EUID);
	run_test(test_export_read_only_UID);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);


int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass) {
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

		//debug!
		dup2(pipefd_out[1], STDERR_FILENO);


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

		// if (!isRunningOnGitHubActions())
		usleep(3000);
        write(pipefd_in[1], command_to_exec.c_str(), command_to_exec.size());
        // write(pipefd_in[1], "\x04", 1);

		// close pipefd_in after use to send the eof
		close(pipefd_in[1]);

		// if (!isRunningOnGitHubActions())
		usleep(3000);

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
		debug("status before transform: %d", status);
		if (WIFEXITED(status)) /* child exited normally */
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) /* child exited on a signal */
			exit_status = WTERMSIG(status) + 128; /* 128 is the offset for signals */
		else
			exit_status = EXIT_FAILURE; /* child exited abnormally (should not happen)*/
		debug("exit status: %d", exit_status);
		return exit_status;
	}
}


bool isRunningOnGitHubActions() 
{
	const char* github_actions = std::getenv("GITHUB_ACTIONS");
	return github_actions != NULL && strcmp(github_actions, "true") == 0;
}