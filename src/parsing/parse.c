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

bool parse(int argc, const char** argv, ls_flags* flags)
{
	int i = 1;
	for (; i < argc && argv[i][0] == '-'; i++)
	{
		if (parse_flags(argv[i], flags) == LS_PARSE_ERROR)
		{
			km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
			return false;
		}
	}
	return true;
}
