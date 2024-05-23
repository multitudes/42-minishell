#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"


/*
This is a test function. It should return NULL if the test passes
debug will print to the log
log_err will print to the error log
my_assert will check a condition and return an error message if it fails
run_test will run a test function and return the error message if it fails
RUN_TESTS will run all the tests and print the results

In this test I will start the minishell and send a Ctrl-D to close it
check the exit status of the minishell
*/
const char* test_basicminishell() {
	// I would like to test my minishell here
	int pipefd[2];
  	my_assert(pipe(pipefd) != -1, "Failed to create pipe");
   
    pid_t pid = fork();
	my_assert(pid != -1, "Failed to fork");

    if (pid == 0) {
        // Child process
        close(pipefd[1]); // Close write end of pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect standard input to read end of pipe
        execl("../minishell", "minishell", (char*) NULL); // Execute minishell
        exit(EXIT_FAILURE); // Exit if execl fails
    } else {
        // Parent process
        close(pipefd[0]); // Close read end of pipe
		write(pipefd[1], "\x04", 1); // Write Ctrl-D to write end of pipe
        close(pipefd[1]); // Close write end of pipe
        int status;
        waitpid(pid, &status, 0); // Wait for child process to exit
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			debug("exit status: %d", WEXITSTATUS(status));
            return "Minishell exited with non-zero status";
        }
    }
	return NULL;
}


const char* test_basicminishell2() {
int pipefd[2];
int pipefd_out[2]; // New pipe for capturing output

pipe(pipefd);
pipe(pipefd_out); // Initialize new pipe

pid_t pid = fork();

if (pid == 0) {
    // Child process
    close(pipefd[1]); // Close write end of input pipe
    dup2(pipefd[0], STDIN_FILENO); // Redirect standard input to read end of input pipe

    close(pipefd_out[0]); // Close read end of output pipe
    dup2(pipefd_out[1], STDOUT_FILENO); // Redirect standard output to write end of output pipe

    execl("../minishell", "minishell", (char*) NULL); // Execute minishell
    exit(EXIT_FAILURE); // Exit if execl fails
} else {
    // Parent process
    close(pipefd[0]); // Close read end of input pipe
    write(pipefd[1], "\x04", 1); // Write Ctrl-D to write end of input pipe
    close(pipefd[1]); // Close write end of input pipe

    close(pipefd_out[1]); // Close write end of output pipe

    // Read the output of the minishell
    char buffer[1024];
    int n = read(pipefd_out[0], buffer, sizeof(buffer)); // Read from read end of output pipe
    buffer[n] = '\0';
    debug("output: -%s-", buffer);

 
	// should be output a string like
	// \nminishell $ exit\n
	my_assert(strcmp(buffer, "minishell $ minishell $ exit\n") == 0, "Output is not as expected");
	// flush the buffer (fprintf)
	fflush(NULL);	

	int status;
	waitpid(pid, &status, 0); // Wait for child process to exit
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		debug("exit status: %d", WEXITSTATUS(status));
		return "Minishell exited with non-zero status";
	}
    }
	return NULL;
}




/*
example to test env variables
for instance check the value of $SHELL that on my mac is zsh
[edit!] this willnot work on different systems so I removed the test 
*/
const char *test_environment(){
	// I would like to test my minishell here
	int pipefd[2];
  	my_assert(pipe(pipefd) != -1, "Failed to create pipe");
   
    pid_t pid = fork();
	my_assert(pid != -1, "Failed to fork");

    if (pid == 0) {
           // Child process
		close(pipefd[1]); // Close write end of pipe
		dup2(pipefd[0], STDIN_FILENO); // Redirect standard input to read end of pipe

		// Get the value of the environment variable
		const char* value = getenv("SHELL");
		debug("value shell env var: %s\n", value);
		// my_assert(value != NULL, "Environment variable MY_VARIABLE is not set");
		// my_assert(strcmp(value, "/bin/zsh") == 0, "Environment variable MY_VARIABLE has incorrect value");
		execl("../minishell", "minishell", (char*) NULL); // Execute minishell

	    exit(EXIT_FAILURE); // Exit if execl fails
    } else {
        // Parent process
        close(pipefd[0]); // Close read end of pipe
		write(pipefd[1], "\x04", 1); // Write Ctrl-D to write end of pipe
        close(pipefd[1]); // Close write end of pipe
        int status;
        waitpid(pid, &status, 0); // Wait for child process to exit
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			debug("exit status: %d", WEXITSTATUS(status));
            return "Minishell exited with non-zero status";
        }
    }

	return NULL;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_basicminishell);
	run_test(test_basicminishell2);
	run_test(test_environment);	
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);

