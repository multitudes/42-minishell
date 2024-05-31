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

int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass) {
    int pipefdin[2];
    int pipefd_out[2]; 

    if (pipe(pipefdin) == -1)
        perror("pipe");
    if (pipe(pipefd_out) == -1)
        perror("pipe");

    pid_t pid = fork();
    
	if (pid == -1)
		perror("fork");
    
    if (pid == 0) {
		if (pipefdin[1] != STDIN_FILENO)
		{
			if (dup2(pipefdin[1], STDIN_FILENO) == -1)
				perror("dup2");
			if (close(pipefdin[1]))
				perror("close");
		}
        if (close(pipefdin[0]) == -1)
			perror("close");
		if (pipefd_out[1] != STDOUT_FILENO)
		{
			if (dup2(pipefd_out[1], STDOUT_FILENO))
				perror("dup2");
			if (close(pipefd_out[1]))
				perror("close");
		}
        execl("../minishell", "minishell", (char*) NULL);
        exit(EXIT_FAILURE);

    } else {
		
        close(pipefd_out[1]);
        close(pipefdin[0]);
        write(pipefdin[1], command_to_exec.c_str(), command_to_exec.size());
        // write(pipefdin[1], "\x04", 1);
		close(pipefdin[1]);
		usleep(2100);

        char buffer[1024];
        int n = read(pipefd_out[0], buffer, sizeof(buffer));
        buffer[n] = '\0';
       	debug("output: -%s-", buffer);
        

        if (strcmp(buffer, expected_output.c_str()) == 0)
			*pass = true;
		debug("pass: %s", pass ? "true" : "false");
		close(pipefd_out[0]);
		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status)) {
			printf("child exited, status=%d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("child killed by signal %d (%s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
		} else if (WIFSTOPPED(status)) {
			printf("child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
		}
		return WEXITSTATUS(status);
	}
}

const char* test_basicminishell_exit() {

	bool pass = false;
	std::string command_to_exec = "exit\n";
	std::string expected_output = "minishell $ exit\nminishell $ minishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell2_echo() {
	bool pass = false;
	std::string command_to_exec = "echo\n";
	std::string expected_output = "minishell $ echo\n\nminishell $ minishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

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
	run_test(test_basicminishell_exit);
	run_test(test_basicminishell2_echo);
	run_test(test_basicminishell3);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);

