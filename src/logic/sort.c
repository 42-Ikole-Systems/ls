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

#include "sort.h"
#include "libkm.h"

#include <stdlib.h>

void sort(operand_list_t** list, ls_flags flags)
{
	// Use bubble sort to sort the list in ascending order
    bool swapped;
    operand_list_t* curr = NULL;
    operand_list_t* prev = NULL;


    do {
        swapped = false;
        curr = *list;

		compare_function_t compare = get_compare_function(flags);
        while (curr && curr->next && curr->next != prev)
		{
            if (compare(curr, curr->next) == true)
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
			prev = curr;
            curr = curr->next;
        }
    } while (swapped);
}


compare_function_t get_compare_function(ls_flags flags)
{
	if (flags & flag_no_sort) {
		return no_sort;
	}
	if (flags & flag_reverse_lexi_sort) {
		return reverse_lexicographical_compare;
	}
	if (flags & flag_modified_time_sort) {
		return time_modified_compare;
	}
	if (flags & flag_access_time_sort) {
		return time_access_compare;
	}
	return lexicographical_compare;
}


bool lexicographical_compare(operand_list_t* left, operand_list_t* right)
{
	return (km_strcmp(left->name, right->name) > 0);
}

bool reverse_lexicographical_compare(operand_list_t* left, operand_list_t* right)
{
	return lexicographical_compare(right, left);
}

bool time_modified_compare(operand_list_t* left, operand_list_t* right)
{
	return (left->statInfo.st_mtime > right->statInfo.st_mtime);
}

bool time_access_compare(operand_list_t* left, operand_list_t* right)
{
	return (left->statInfo.st_atime > right->statInfo.st_atime);
}

bool no_sort(operand_list_t* left, operand_list_t* right)
{
	(void)left;
	(void)right;
	return false;
}
