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
#include "file.h"
#include "sort.h"
#include "libkm.h"

#include <unistd.h>

static int set_operand_types(operand_list_t* operands)
{
	for (operand_list_t* node = operands; node != NULL; node = node->next)
	{
		if (set_stat_info(node) == false)
		{
			clear_list(operands);
			return LS_ERROR;
		}
	}
	return true;
}

/*!
 * @brief splits operands into files and directories (by type)
 * @param operands -
 * @param files make sure it is empty and non NULL
 * @param directories make sure it is empty and non NULL 
 * 
 * NOTE: operands will be NULL after calling this
*/
static void split_operands(operand_list_t** operands, operand_list_t** files, operand_list_t** directories)
{
	for (operand_list_t* node = *operands; node != NULL; node = node->next)
	{
		if (node->type == directory_type) {
			list_append_node(directories, node);
		}
		else {
			list_append_node(files, node);
		}
	}
	*operands = NULL;
}

static void print_operands(const operand_list_t* files, const operand_list_t* directories)
{
	if (files) {
		km_printf("directory: %s\n", files->name);
	}
	if (directories) {
		km_printf("directory: %s\n", directories->name);
	}
}

int list_operands(operand_list_t* operands, ls_flags flags)
{
	if (set_operand_types(operands) == LS_ERROR) {
		return LS_ERROR;
	}

	operand_list_t* files = NULL;
	operand_list_t* directories = NULL;
	sort(&operands, flags);
	split_operands(&operands, &files, &directories);

	print_operands(files, directories);
	clear_list(files);
	clear_list(directories);
	return 0;
}
