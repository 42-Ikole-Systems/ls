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

void parse(int argc, const char** argv, ls_flags* flags, km_vector_file* operands)
{
	int i = 1;

	// parse flags first
	for (; status_success() && i < argc && argv[i][0] == '-' && argv[i][1]; i++)
	{
		parse_flags(argv[i], flags);
	}
	// parse operands next
	for (; status_success() && i < argc; i++)
	{
		add_file(NULL, argv[i], operands);
	}

	if (!status_success()) 
	{
		km_vector_file_destroy(operands);
		km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
	}
}
