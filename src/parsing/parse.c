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

int add_operand(const char* dir, const char* operand, operand_list_t** operand_list)
{
	operand_list_t* newNode = list_append(operand_list, dir, operand);
	if (newNode == NULL) {
		return LS_ERROR;
	}
	newNode->type = UNKNOWN_TYPE;
	return true;
}

bool parse(int argc, const char** argv, ls_flags* flags, operand_list_t** operand_list)
{
	bool error = false;
	int i = 1;

	// parse flags first
	for (; error == false && i < argc && argv[i][0] == '-'; i++)
	{
		if (parse_flags(argv[i], flags) == LS_PARSE_ERROR)
		{
			error = true;
		}
	}
	// parse operands next
	for (; error == false && i < argc; i++)
	{
		if (add_operand(".", argv[i], operand_list) == LS_ERROR)
		{
			error = true;
			clear_list(*operand_list);
		}
	}

	if (error == true) {
		km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
	}
	return !error;
}
