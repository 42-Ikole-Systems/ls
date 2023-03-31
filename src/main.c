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
#include "libkm.h"

int main(int argc, const char** argv)
{
	ls_flags flags = 0x00;
	operand_list_t* operand_list = NULL;

	if (parse(argc, argv, &flags, &operand_list) == false) {
		return LS_PARSE_ERROR;
	}

	if (operand_list == NULL)
	{
		// add current directory if no operand is given
		if (list_append(&operand_list, ".") == NULL) {
			return LS_ERROR;
		}
	}

	return list_operands(operand_list, flags);
}
