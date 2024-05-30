#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include <fstream>

const char* test_basicminishell() {

	int pipefdin[2];
	int pipefd_out[2]; // New pipe for capturing output

	if (pipe(pipefdin) == -1)
		return "Failed to create pipe";
	if (pipe(pipefd_out) == -1)
		return "Failed to create second pipe"; // Initialize new pipe

	pid_t pid = fork();
	my_assert(pid != -1, "Failed to fork");
	
	if (pid == 0) {
		// Child process
		dup2(pipefdin[0], STDIN_FILENO); // Redirect standard input to read end of input pipe
		if (close(pipefdin[0]) == -1)
			return "Failed to close pipefd"; // Close read end of input pipe
		if (close(pipefdin[1]) == -1)
			return "Failed to close pipefd"; // Close write end of input pipe
		if (dup2(pipefd_out[1], STDOUT_FILENO) == -1)
			return "dup failed"; // Redirect standard output to write end of output pipe
		if (close(pipefd_out[1]) == -1)
			return "Failed to close pipefd_out"; // Close write end of output pipe
		if (close(pipefd_out[0]) == -1)
			return "Failed to close pipefd_out"; // Close read end of output pipe
		execl("../minishell", "minishell", (char*) NULL); // Execute minishell
		exit(EXIT_FAILURE); // Exit if execl fails
	} else {
		// Parent process
		close(pipefdin[0]); // Close read end of input pipe
		// usleep(1); // Wait for child process to start
		write(pipefdin[1], "echo home\n", 11); // Write Ctrl-D to write end of input pipe
		write(pipefdin[1], "\x04", 1); // Write Ctrl-D to write end of input pipe
		close(pipefdin[1]); // Close write end of input pipe


		// Read the output of the minishell
		char buffer[1024];
		int n = read(pipefd_out[0], buffer, sizeof(buffer)); // Read from read end of output pipe
		// wait here a moment
		// sleep(1);
		buffer[n] = '\0';
		debug("output: -%s-", buffer);

		my_assert((strcmp(buffer, "minishell $ echo home\n") == 0 ), "Output is not as expected");
		close(pipefd_out[0]); // Close read end of output pipe
		close(pipefd_out[1]); // Close write end of output pipe

		int status;
		waitpid(pid, &status, 0); // Wait for child process to exit
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
			debug("exit status: %d", WEXITSTATUS(status));
			return "Minishell exited with non-zero status";
		}
		debug("exit status: %d", WEXITSTATUS(status));
    }
	return NULL;
}


const char *test_basicminishell2() {
    
	return NULL;
}

const char *test_basicminishell3() {
    
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_basicminishell);
	run_test(test_basicminishell2);
	run_test(test_basicminishell3);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);

