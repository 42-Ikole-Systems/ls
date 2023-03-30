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

#include <unistd.h>

static int set_operand_types(operand_list_t* operands)
{
	for (operand_list_t* node = operands; node != NULL; node = node->next)
	{
		node->type = get_file_type(node->name);
		if (node->type == UNKNOWN_TYPE)
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

static void lexicographical_sort(operand_list_t** list)
{
	// Use bubble sort to sort the list in ascending order
    bool swapped;
    operand_list_t* curr = NULL;
    operand_list_t* prev = NULL;
    do {
        swapped = false;
        curr = *list;
        while (curr->next != prev) 
		{
            if (km_strcmp(curr->name, curr->next->name) > 0)
			{
				operand_list_t tmp = *(curr->next);
				if (prev == NULL)
				{
					*list = curr->next;
					curr->next->next = curr;
					curr->next = tmp.next;
				}
				else
				{
					prev->next = curr->next;
					curr->next->next = curr;
					curr->next = tmp.next;
				}
                swapped = true;
            }
            curr = curr->next;
        }
        prev = curr;
    } while (swapped);
}

int list_operands(operand_list_t* operands)
{
	if (set_operand_types(operands) == LS_ERROR) {
		return LS_ERROR;
	}

	operand_list_t* files;
	operand_list_t* directories;
	lexicographical_sort(&operands);
	split_operands(&operands, &files, &directories);


}
