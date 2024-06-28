#include "razorclam_tests.h"
#include <string>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/minishell.h"
#include "../include/globbing.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));

const char *test_globbing_get_files()
{

	// create a tmp directory with 4 files named hello.txt world.c .shellignore and miniclam
	std::system("mkdir tmp");
    std::system("touch tmp/hello.txt tmp/world.c tmp/.shellignore tmp/miniclam");
   
	// save old directory
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

    if (chdir("tmp") == -1)
		perror("chdir");
	t_darray *files = darray_create(sizeof(char *), 100);
	int result = match_files_in_directory(files, ".*");
	debug("result: %d", result);
	debug("files: %s", (char *)darray_get(files, 0));
	std::string expected_file = ".shellignore";
	my_assert(expected_file.compare((char *)darray_get(files, 0)) == 0, "match_files_in_directory failed");
	my_assert(files->end == 1, "match_files_in_directory failed");

	// free the darray
	darray_clear(files);
	result = match_files_in_directory(files, "hello.*");
	debug("result: %d", result);
	debug("files: %s", (char *)darray_get(files, 0));
	expected_file = "hello.txt";
	my_assert(expected_file.compare((char *)darray_get(files, 0)) == 0, "match_files_in_directory failed");
	my_assert(files->end == 1, "match_files_in_directory failed");

	darray_clear(files);
	result = match_files_in_directory(files, "*o*");
	debug("result: %d", result);
	debug("number of files in directory: %d\n", files->end);
	my_assert(files->end == 3, "match_files_in_directory failed");
	
	darray_clear(files);
	result = match_files_in_directory(files, "none.*");
	debug("result: %d", result);
	debug("number of files in directory: %d\n", files->end);
	my_assert(files->end == 0, "match_files_in_directory failed");

	darray_clear_destroy(files);
	
	// remove the tmp directory
	std::system("rm -rf tmp");

	// revert to previous directory
	if (chdir(cwd) == -1)
		perror("chdir");

	return NULL;
}


const char *all_tests()
{
	// necessary to start the test suite
	suite_start();
	
	// run the tests
	run_test(test_globbing_get_files);

	return NULL;
}

// works as a main
RUN_TESTS(all_tests);







//avoiding adding the whole libft only for this
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	**l;
	t_list	*temp;

	if (lst == NULL || *lst == NULL)
		return ;
	l = lst;
	temp = *lst;
	while ((*lst)->next)
	{
		*lst = (*lst)->next;
		ft_lstdelone(temp, del);
		temp = *lst;
	}
	ft_lstdelone(temp, del);
	*l = NULL;
}
