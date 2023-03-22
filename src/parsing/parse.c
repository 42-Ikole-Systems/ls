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

bool parse(int argc, const char** argv, ls_flags* flags, operand_list_t** operand_list)
{
	bool success = true;
	int i = 1;

	// parse flags first
	for (; success == true && i < argc && argv[i][0] == '-'; i++)
	{
		if (parse_flags(argv[i], flags) == LS_PARSE_ERROR)
		{
			success = false;
		}
	}
	// parse operands next
	for (; success == true && i < argc; i++)
	{
		if (parse_operand(argv[i], operand_list) == LS_PARSE_ERROR)
		{
			success = false;
			clear_list(*operand_list);
		}
	}

	if (success == false) {
		km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
	}
	return success;
}
