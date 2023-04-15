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

ls_status add_operand(const char* dir, const char* operand, operand_list_t** operand_list)
{
	operand_list_t* newNode = list_append(operand_list, dir, operand);
	if (newNode == NULL) {
		return LS_SERIOUS_ERROR;
	}
	newNode->type = UNKNOWN_TYPE;
	return LS_SUCCESS;
}

ls_status parse(int argc, const char** argv, ls_flags* flags, operand_list_t** operand_list)
{
	ls_status status = LS_SUCCESS;
	*operand_list = NULL;
	int i = 1;

	// parse flags first
	for (; status == LS_SUCCESS && i < argc && argv[i][0] == '-'; i++)
	{
		status = parse_flags(argv[i], flags);
	}
	// parse operands next
	for (; status == LS_SUCCESS && i < argc; i++)
	{
		status = add_operand(NULL, argv[i], operand_list);
	}

	if (status != LS_SUCCESS) {
		clear_list(*operand_list);
		km_printf("usage: ls [-%s] [file]\n", ALLOWED_LS_FLAGS);
	}
	return status;
}
