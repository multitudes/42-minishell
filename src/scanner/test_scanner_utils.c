#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int run_command_and_check_output(const char* command_to_exec, const char* expected_output, bool* pass);

int main() {
    bool pass = false;
    
    // Test case 1: Command output matches expected output
    int exit_status = run_command_and_check_output("ls", "file1.txt\nfile2.txt\nfile3.txt\n", &pass);
    if (pass && exit_status == 0) {
        printf("Test case 1 passed\n");
    } else {
        printf("Test case 1 failed\n");
    }
    
    // Test case 2: Command output does not match expected output
    exit_status = run_command_and_check_output("ls", "file1.txt\nfile2.txt\n", &pass);
    if (!pass && exit_status == 0) {
        printf("Test case 2 passed\n");
    } else {
        printf("Test case 2 failed\n");
    }
    
    // Test case 3: Command execution fails
    exit_status = run_command_and_check_output("invalid_command", "", &pass);
    if (!pass && exit_status != 0) {
        printf("Test case 3 passed\n");
    } else {
        printf("Test case 3 failed\n");
    }
    
    return 0;
}