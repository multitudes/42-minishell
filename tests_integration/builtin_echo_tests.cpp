#include "razorclam_tests.h"
#include <string>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"

#include <string>
#include <cstring>

// forward declaration
int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass);
bool isRunningOnGitHubActions(); 


const char *test_basicminishell_echo() {
	bool pass = false;
	std::string command_to_exec = "echo\n";
	std::string expected_output = "minishell $ echo\n\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell_echo2() {
	bool pass = false;
	std::string command_to_exec = "echo -n\n";
	std::string expected_output = "minishell $ echo -n\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell_echo3() {
	bool pass = false;
	std::string command_to_exec = "echo -nnn -nn\n";
	std::string expected_output = "minishell $ echo -nnn -nn\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell_echo4() {
	bool pass = false;
	std::string command_to_exec = "echo -nnn -nn -mnnn hello\n";
	std::string expected_output = "minishell $ echo -nnn -nn -mnnn hello\n-mnnn hellominishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
    debug("expected_output: %s", expected_output.c_str());
    debug("command_to_exec: %s", command_to_exec.c_str());
    debug("status: %d and pass %s", status, pass ? "true" : "false");
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");

	return NULL;
}


const char *test_basicminishell_echo5() {
	bool pass = false;
	std::string command_to_exec = "echo -mnnn hello\n";
	std::string expected_output = "minishell $ echo -mnnn hello\n-mnnn hello\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell_echo6() {
	bool pass = false;
	std::string command_to_exec = "echo $NonExistingVar\n";
	std::string expected_output = "minishell $ echo $NonExistingVar\n\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}


const char *test_basicminishell_echo7() {
	if (getenv("HOME") != NULL)
	{
		bool pass = false;
		std::string command_to_exec = "echo $HOME\n";
		std::string my_home = getenv("HOME");
		std::string expected_output = "minishell $ echo $HOME\n" + my_home + "\nminishell $ exit\n";
		int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
		my_assert(status == 0, "Minishell exited with non-zero status");
		my_assert(pass, "Output is not as expected");
		debug("command_to_exec: %s", command_to_exec.c_str());
		debug("expected_output: %s", expected_output.c_str());
		debug("status: %d and pass %s", status, pass ? "true" : "false");
	}
	return NULL;
}

const char *test_basicminishell_echo8() {
	if (getenv("HOME") != NULL)
	{
		bool pass = false;
		std::string command_to_exec = "echo $HOMe\n";
		std::string expected_output = "minishell $ echo $HOMe\n\nminishell $ exit\n";
		int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
		my_assert(status == 0, "Minishell exited with non-zero status");
		my_assert(pass, "Output is not as expected");
		debug("command_to_exec: %s", command_to_exec.c_str());
		debug("expected_output: %s", expected_output.c_str());
		debug("status: %d and pass %s", status, pass ? "true" : "false");
	}

	return NULL;
}

//	std::string command_to_exec = "echo ~\n";
const char *test_basicminishell_echo9() {

	if (getenv("HOME") != NULL)
	{
		bool pass = false;
		std::string command_to_exec = "echo ~\n";
		std::string my_home = getenv("HOME");
		std::string expected_output = "minishell $ echo ~\n" + my_home + "\nminishell $ exit\n";
		int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
		my_assert(status == 0, "Minishell exited with non-zero status");
		my_assert(pass, "Output is not as expected");
		debug("command_to_exec: %s", command_to_exec.c_str());
		debug("expected_output: %s", expected_output.c_str());
		debug("status: %d and pass %s", status, pass ? "true" : "false");
	}
	
	return NULL;
}

const char *test_basicminishell_echo10() {
	bool pass = false;
	std::string command_to_exec = "echo $PATH\n";
	std::string my_path = getenv("PATH");
	std::string expected_output = "minishell $ echo $PATH\n" + my_path + "\nminishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}

const char *test_basicminishell_echo11() {
	bool pass = false;
	std::string command_to_exec = "echo -n hello\n";
	std::string expected_output = "minishell $ echo -n hello\nhellominishell $ exit\n";
	int status = run_command_and_check_output(command_to_exec, expected_output, &pass);
	my_assert(status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
	debug("command_to_exec: %s", command_to_exec.c_str());
	debug("expected_output: %s", expected_output.c_str());
	debug("status: %d and pass %s", status, pass ? "true" : "false");

	return NULL;
}

const char *test_basicminishell3() {
	// I want to test the following commands in bash with 
	// popen ... like this: "echo\n"


	return NULL;
}

const char *all_tests()
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

	run_test(test_basicminishell3);
	
	return NULL;
}

// works as a main
RUN_TESTS(all_tests);




int	run_command_and_check_output(const std::string& command_to_exec, const std::string& expected_output, bool *pass) {
	// seen from the point of you of the child process. pipefd_in is the input to the child process
	// and pipefd_out is the output of the child process
	int status;
	int	exit_status;
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
		
		if (!isRunningOnGitHubActions())
			usleep(5000);
        else 
            usleep(2000);

        write(pipefd_in[1], command_to_exec.c_str(), command_to_exec.size());
        // write(pipefd_in[1], "\x04", 1);

		// close pipefd_in after use to send the eof
		close(pipefd_in[1]);
		
        if (!isRunningOnGitHubActions())
            usleep(5000);
        else 
            usleep(2000);
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


bool isRunningOnGitHubActions() 
{
    const char* githubActions = std::getenv("GITHUB_ACTIONS");
    return githubActions != nullptr && githubActions == std::string("true");
}
