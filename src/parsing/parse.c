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

#include "parsing/parse.h"
#include "utility/file.h"

#include "libkm/io/printf.h"
#include "libkm/string.h"

#include <ctype.h>

ls_status parse(int argc, const char** argv, ls_flags* flags, km_vector_file* operands)
{
	ls_status status = LS_SUCCESS;
	int i = 1;

	// parse flags first
	for (; status == LS_SUCCESS && i < argc && argv[i][0] == '-' && argv[i][1]; i++)
	{
		status = parse_flags(argv[i], flags);
	}
	// parse operands next
	for (; status == LS_SUCCESS && i < argc; i++)
	{
		status = add_file(NULL, argv[i], operands);
	}

	if (status != LS_SUCCESS) 
	{
		km_vector_file_destroy(&operands);
		km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
	}
	return status;
}
