/* ************************************************************************** */
/*                                                                            */
/*                    _____                        _____                      */
/*                   |\    \                  _____\    \                     */
/*                    \\    \                /    / \    |                    */
/*                     \\    \              |    |  /___/|                    */
/*                      \|    | ______   ____\    \ |   ||                    */
/*                       |    |/      \ /    /\    \|___|/                    */
/*                       /            ||    |/ \    \                         */
/*                      /_____/\_____/||\____\ /____/|                        */
/*                     |      | |    ||| |   ||    | |                        */
/*                     |______|/|____|/ \|___||____|/                         */
/*                                                                            */
/*                   Kingmar  |  https://github.com/K1ngmar                   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include "logic/sort.h"
#include "logic/operands.h"
#include "parsing/parse.h"
#include "utility/file.h"

#include <libkm/io/printf.h>
#include <libkm/string.h>
#include <libkm/memory.h>

#include <stdlib.h>

REGISTER_KM_VECTOR_SOURCE(ls_file, file)

void run_leaks(const char* executableName)
{
	km_printf("\n");
	const char* programname = km_strrchr(executableName, '/');
	if (programname == NULL) {
		programname = executableName;
	}
	else {
		// skip '/'
		programname++;
	}
	char* leaks = NULL;
	km_sprintf(&leaks, "leaks %s | grep 'leaks for'", programname);
	system(leaks);
	free(leaks);
}

int main(int argc, const char** argv)
{
	ls_flags flags;
	km_vector_file operands;

	km_bzero(&flags, sizeof(ls_flags));
	km_vector_file_initialise(&operands, destroy_file, NULL);

	parse(argc, argv, &flags, &operands);
	if (!status_success()) {
		return get_global_status();
	}

	if (operands.size == 0)
	{
		// add current directory if no operand is given
		add_file(NULL, ".", &operands);
		if (!status_success()) {
			return get_global_status();
		}
	}

	set_operand_data(&operands, flags);
	if (!status_success()) {
		return get_global_status();
	}

	km_vector_file files;
	km_vector_file directories;
	sort(&operands, flags);
	split_operands(&operands, &files, &directories);
	if (!status_success()) {
		return get_global_status();
	}

	list_operands(&files, &directories, flags);
	km_vector_file_destroy(&files);
	km_vector_file_destroy(&directories);

	#ifdef LEAKS
		run_leaks(argv[0]);
	#endif
	return get_global_status();
}
