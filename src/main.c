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
#include "libkm/io/printf.h"
#include "libkm/string.h"
#include "libkm/memory.h"
#include "logic/sort.h"
#include "logic/operands.h"
#include "parsing/parse.h"
#include "utility/list.h"

#include <stdlib.h>

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
	km_bzero(&flags, sizeof(ls_flags));
	operand_list_t* operands = NULL;
	ls_status status = LS_SUCCESS;

	status = parse(argc, argv, &flags, &operands);
	if (status != LS_SUCCESS) {
		return status;
	}

	if (operands == NULL)
	{
		// add current directory if no operand is given
		if (list_append(&operands, NULL, ".") == NULL) {
			return LS_SERIOUS_ERROR;
		}
	}

	status = set_operand_data(&operands, flags);
	if (status != LS_SUCCESS) {
		return status;
	}

	operand_list_t* files = NULL;
	operand_list_t* directories = NULL;
	sort(&operands, flags);
	split_operands(&operands, &files, &directories);

	status = list_operands(files, directories, flags, 0);
	clear_list(files);
	clear_list(directories);

	#ifdef LEAKS
		run_leaks(argv[0]);
	#endif
	return status;
}
