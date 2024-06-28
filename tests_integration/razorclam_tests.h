#ifndef __razorclam_tests_h__
# define __razorclam_tests_h__

# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include <limits.h>

/*
This allows us to use the debug macro to print debug messages but to 
compile them out when NDEBUG is defined.
If we define NDEBUG in the makefile or as a flag -DNDEBUG, 
the debug macro will be replaced with an empty macro.
during submission we will remove the debug macro from the code
including the macro below which is not allowed by norminette.
*/
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%s:%d: " M "\n",\
        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr,\
        "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,\
        clean_errno(), ##__VA_ARGS__)

// for the test suite
#define suite_start() const char *message = NULL

#define my_assert(test, message) if (!(test)) {\
log_err(message); return message; }

#define run_test(test) debug("\n-----%s", " " #test); \
message = test(); tests_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc, char *argv[]) {\
argc = 1; \
(void)argc; \
debug("----- RUNNING: %s", argv[0]);\
printf("----\nRUNNING: %s\n", argv[0]);\
const char *result = name();\
if (result != 0) {\
printf("FAILED: %s\n", result);\
}\
else {\
printf("ALL TESTS PASSED\n");\
}\
printf("Tests run: %d\n", tests_run);\
exit(result != 0);\
}

int tests_run;



#endif
