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


std::string get_current_directory() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return std::string(buffer);
    } else {
        // handle error
        std::cerr << "Failed to get current directory" << std::endl;
        return "";
    }
}

/*
Ex output: -minishell $ echo $PWD
/home/lbrusa/DEV/minishell/tests_integration
minishell $ exit
-
*/
const char* test_pwd_on_startup() 
{
    bool pass = false;
	uint8_t	exit_status;
	std::string output = "minishell $ echo $PWD\n" + get_current_directory();
    exit_status = run_command_and_check_output("echo $PWD", output, &pass);
	my_assert(exit_status == 0, "Minishell exited with non-zero status");
	my_assert(pass, "Output is not as expected");
    return NULL;
}

// comment out because apparently the output is mixedd with the sterr?
/*
 DEBUG builtin_pwd_tests.cpp:run_command_and_check_output:152: output: -minishell $ unset PWD
make[1]: *** [Makefile:71: runtests] Error 1
minishell $ 
minishell $ echo $PWD

minishell $ exit
make: *** [Makefile:90: tests_integration] Error 2
-
*/

// const char* test_pwd_after_unset() {
//     bool pass = false;
// 	uint8_t	exit_status;
// 	std::string output = "minishell $ unset PWD\nminishell $ \necho $PWD\n\nminishell $ exit\n";
//     exit_status = run_command_and_check_output("unset PWD\n\necho $PWD", output, &pass);
// 	my_assert(exit_status == 0, "Minishell exited with non-zero status");
// 	my_assert(pass, "Output is not as expected");
//     return NULL;
// }
// const char* test_pwd_after_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd /path/to/new/directory; echo $PWD", "/path/to/new/directory", &pass);
//     return pass ? NULL : "PWD not updated after cd";
// }

// const char* test_pwd_after_non_cd_command() {
//     bool pass = false;
//     run_command_and_check_output("ls; echo $PWD", "/path/to/initial/directory", &pass);
//     return pass ? NULL : "PWD changed after non-cd command";
// }

// const char* test_pwd_after_relative_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd relative/path; echo $PWD", "/path/to/initial/directory/relative/path", &pass);
//     return pass ? NULL : "PWD not updated correctly after relative cd";
// }

// const char* test_pwd_after_absolute_cd() {
//     bool pass = false;
//     run_command_and_check_output("cd /absolute/path; echo $PWD", "/absolute/path", &pass);
//     return pass ? NULL : "PWD not updated correctly after absolute cd";
// }

const char *all_tests() {
    suite_start();
    run_test(test_pwd_on_startup);
	// run_test(test_pwd_after_unset);
    // run_test(test_pwd_after_cd);
    // run_test(test_pwd_after_non_cd_command);
    // run_test(test_pwd_after_relative_cd);
    // run_test(test_pwd_after_absolute_cd);
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
		usleep(3000);
        write(pipefd_in[1], command_to_exec.c_str(), command_to_exec.size());
        // write(pipefd_in[1], "\x04", 1);

		// close pipefd_in after use to send the eof
		close(pipefd_in[1]);
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
		if (WIFEXITED(status)) /* child exited normally */
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status)) /* child exited on a signal */
			exit_status = WTERMSIG(status) + 128; /* 128 is the offset for signals */
		else
			exit_status = EXIT_FAILURE; /* child exited abnormally (should not happen)*/
		return exit_status;
	}
}

bool isRunningOnGitHubActions() {
	const char* github_actions = std::getenv("GITHUB_ACTIONS");
	return github_actions != NULL && strcmp(github_actions, "true") == 0;
}