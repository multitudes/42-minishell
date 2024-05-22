#include "razorclam_tests.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/scanner.h"
#include "../include/darray.h"


static t_darray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

const char* test_create() 
{
	array = darray_create(sizeof(int), 100);
	my_assert(array != NULL, "DArray_create failed.");
	my_assert(array->contents != NULL, "contents are wrong in darray");
	my_assert(array->end == 0, "end isn't at the right spot");
	my_assert(array->element_size == sizeof(int),
			  "element size is wrong.");
	my_assert(array->max == 100, "wrong max length on initial size");
	
	return NULL;
}

/*
darray_destroy(array) contains:
	if (array) {
	if (array->contents)
		free(array->contents);
	free(array);
}
*/
const char *test_destroy()
{
	// no my_assert here?
	darray_destroy(array);
	// how to check something been freed?
	
	return NULL;
}

/*
using new element to create a new element in the array
but only for those of fixed size, strings should be allocated
on the heap and then added to the array with push or set
*/
const char *test_new()
{
	// a int pointer to the first el of darray
	val1 = (int*)darray_new_elem(array);
	my_assert(val1 != NULL, "failed to make a new element");
	debug("val1 pointer %p with val %d",val1, *(int*)val1 );
	
	val2 = (int *)darray_new_elem(array);
	my_assert(val2 != NULL, "failed to make a new element");
	debug("val2 pointer %p val %d",val2, *(int*)val2);
	
	return NULL;
}

const char *test_set()
{
	// set will be assigning val1 to array[0] and val2 to array[1]
	darray_set(array, 0, val1);
	darray_set(array, 1, val2);
	
	return NULL;
}

const char *test_get()
{
	// check that the previous values are in
	my_assert(darray_get(array, 0) == val1, "Wrong first value.");
	my_assert(darray_get(array, 1) == val2, "Wrong second value.");
	
	return NULL;
}

const char *test_remove()
{
	int *val_check = (int *)darray_remove(array, 0);
	my_assert(val_check != NULL, "Should not get NULL.");
	my_assert(*val_check == *val1, "Should get the first value.");
	my_assert(darray_get(array, 0) == NULL, "Should be gone.");
	darray_free(val_check);
	
	val_check = (int *)darray_remove(array, 1);
	my_assert(val_check != NULL, "Should not get NULL.");
	my_assert(*val_check == *val2, "Should get the first value.");
	my_assert(darray_get(array, 1) == NULL, "Should be gone.");
	darray_free(val_check);
	
	return NULL;
}

const char *test_expand_contract()
{
	int old_max = array->max;
	darray_expand(array);
	my_assert((unsigned int)array->max == old_max + array->expand_rate,
			  "Wrong size after expand.");
	
	darray_contract(array);
	my_assert((unsigned int)array->max == array->expand_rate + 1,
			  "Should stay at the expand_rate at least.");
	
	darray_contract(array);
	my_assert((unsigned int)array->max == array->expand_rate + 1,
			  "Should stay at the expand_rate at least.");
	
	return NULL;
}

const char *test_push_pop()
{
	int i = 0;
	for (i = 0; i < 1000; i++) {
		int *val = (int *)darray_new_elem(array);
		*val = i * 333;
		darray_push(array, val);
	}
	
	my_assert(array->max == 1201, "Wrong max size.");
	
	for (i = 999; i >= 0; i--) {
		int *val = (int *)darray_pop(array);
		my_assert(val != NULL, "Shouldn't get a NULL.");
		my_assert(*val == i * 333, "Wrong value.");
		darray_free(val);
	}
	
	return NULL;
}

/*
testing the environment

*/ 
extern char **environ;// = {"RACK_DIR=/Users/laurentb/Documents/SynthLab/Rack-SDK","MANPATH=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/share/man:/Library/Developer/CommandLineTools/usr/share/man:/Library/Developer/CommandLineTools/Toolchains/XcodeDefault.xctoolchain/usr/share/man:/opt/homebrew/share/man::","TERM_PROGRAM=iTerm.app","TERM=xterm-256color","SHELL=/bin/zsh","MAKEFLAGS=","HOMEBREW_REPOSITORY=/opt/homebrew","TMPDIR=/var/folders/r3/1nj81sp171s6byfnnmngryg40000gn/T/","CONDA_SHLVL=1","LIBRARY_PATH=/opt/homebrew/lib","TERM_PROGRAM_VERSION=3.4.23","CONDA_PROMPT_MODIFIER=(base) ","TERM_SESSION_ID=w0t0p0:8805CD47-A184-4E88-A9FF-DB99865DE934","USER=laurentb","COMMAND_MODE=unix2003","CONDA_EXE=/Users/laurentb/anaconda3/bin/conda","SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.K4Vw5bQrlt/Listeners","CPATH=/opt/homebrew/opt/boost/include:","__CF_USER_TEXT_ENCODING=0x1F5:0x0:0x0","MAKELEVEL=2","MFLAGS=","_CE_CONDA=","PATH=/Users/laurentb/anaconda3/bin:/Users/laurentb/anaconda3/condabin:/Applications/usdpython/usdzconvert:/Applications/usdpython/USD:/opt/homebrew/opt/boost/bin:/opt/homebrew/bin:/opt/homebrew/sbin:/Library/Frameworks/Python.framework/Versions/3.8/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Users/laurentb/Library/Python/3.8/bin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/local/bin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/bin:/var/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/appleinternal/bin:/opt/X11/bin:/Library/Apple/usr/bin:/Library/TeX/texbin:/usr/local/go/bin:/Users/laurentb/.cargo/bin:/Users/laurentb/.local/bin:/usr/local/bin:/Users/laurentb/.local/bin","LaunchInstanceID=350E61B5-C001-4F3D-B8B2-C28D4DE66F53","CONDA_PREFIX=/Users/laurentb/anaconda3","__CFBundleIdentifier=com.googlecode.iterm2","PWD=/Users/laurentb/Developer/beash-minishell/tests","LANG=en_US.UTF-8","ITERM_PROFILE=Default","XPC_FLAGS=0x0","_CE_M=","XPC_SERVICE_NAME=0","COLORFGBG=7;0","SHLVL=2","HOME=/Users/laurentb","LC_TERMINAL_VERSION=3.4.23","HOMEBREW_PREFIX=/opt/homebrew","ITERM_SESSION_ID=w0t0p0:8805CD47-A184-4E88-A9FF-DB99865DE934","CONDA_PYTHON_EXE=/Users/laurentb/anaconda3/bin/python","PYTHONPATH=/Applications/usdpython/USD/lib/python:","LOGNAME=laurentb","CONDA_DEFAULT_ENV=base","HOMEBREW_CELLAR=/opt/homebrew/Cellar","INFOPATH=/opt/homebrew/share/info:","LC_TERMINAL=iTerm2","DISPLAY=/private/tmp/com.apple.launchd.kVbCp5Pi8N/org.xquartz:0","SECURITYSESSIONID=19b8d","COLORTERM=truecolor","_=./minishell_basic1_tests"};

const char *test_env()
{
	t_data *data;
	t_darray *env = NULL;
	data = NULL;

	// init data structure with environ and path
	if (!init_data(&data))
		return "Failed to init data";
	env = data->env_arr;
	int i;
	i = 0;	
	while (environ[i])
	{
		debug("environ[%d] = %s", i, environ[i]);
		debug("env[%d] = %s", i, (char *)darray_get(env, i));
		if (strcmp(environ[i], (char *)darray_get(env, i)) != 0 )
		{
			return "Failed to copy environ to env_arr";
		}
		i++;
	}

	return NULL;	
}

/*
remove_env(env, "_");
add_env(env, "new", "value");

in c and c++ :getenv("_"); will return the value of the last command
but ion our data struct we store the env variables and we use our
mini_get_env function to get the value of the env variable
*/
const char *test_env_update()
{

	t_data *data;
	data = NULL;

	// init data structure with environ and path
	if (!init_data(&data))
		return "Failed to init data";	
	update_env(data, "_", "./value");
	const char *env = mini_get_env(data, "_");
	if (strcmp(env, "./value") != 0)
	{
		return "Failed to update _ env var";
	}
	
	return NULL;
}

const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_create);	
	run_test(test_new);
	run_test(test_set);
	run_test(test_get);
	run_test(test_remove);
	run_test(test_expand_contract);
	run_test(test_push_pop);	
	run_test(test_destroy);
	run_test(test_env);
	run_test(test_env_update);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);
