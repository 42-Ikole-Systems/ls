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

#include "parse.h"
#include "libkm.h"

#include <ctype.h>

bool parse(int argc, const char** argv, ls_flags* flags, operand_list_t* fileOperands, operand_list_t* nonFileOperands)
{
	int i = 1;
	// parse flags first
	for (; i < argc && argv[i][0] == '-'; i++)
	{
		if (parse_flags(argv[i], flags) == LS_PARSE_ERROR)
		{
			km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
			return false;
		}
	}
	// parse operands next
	for (; i < argc; i++)
	{
		if (parse_operand(argv[i], fileOperands, nonFileOperands) == LS_PARSE_ERROR)
		{
			clear_list(fileOperands);
			clear_list(nonFileOperands);
			return false;
		}
	}
	return true;
}
