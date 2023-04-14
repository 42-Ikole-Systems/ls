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
#include "logic/sort.h"
#include "logic/operands.h"

int main(int argc, const char** argv)
{
	ls_flags flags = 0x00;
	operand_list_t* operands = NULL;

	if (parse(argc, argv, &flags, &operands) == false) {
		return LS_PARSE_ERROR;
	}

	if (operands == NULL)
	{
		// add current directory if no operand is given
		if (list_append(&operands, NULL, ".") == NULL) {
			return LS_ERROR;
		}
	}

	int status = LS_SUCCESS;
	status = set_operand_data(operands, flags);
	if (status == LS_SUCCESS)
	{
		operand_list_t* files = NULL;
		operand_list_t* directories = NULL;
		sort(&operands, flags);
		split_operands(&operands, &files, &directories);

		status = list_operands(files, directories, flags, 0);
		clear_list(files);
		clear_list(directories);
		clear_list(operands);
	}

	return status;
}
