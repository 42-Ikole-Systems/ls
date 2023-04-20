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

#include "logic/sort.h"
#include "libkm.h"

#include <stdlib.h>

void sort(operand_list_t** list, ls_flags flags)
{
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
		prev = NULL;
    } while (swapped);
}


compare_function_t get_compare_function(ls_flags flags)
{
	if (flags.no_sort) {
		return no_sort;
	}
	if (flags.reverse_lexi_sort) {
		return reverse_lexicographical_compare;
	}
	if (flags.time_sort) {
		return time_compare;
	}
	return lexicographical_compare;
}


bool lexicographical_compare(operand_list_t* left, operand_list_t* right)
{
	return (km_strcmp(left->filename, right->filename) > 0);
}

bool reverse_lexicographical_compare(operand_list_t* left, operand_list_t* right)
{
	return lexicographical_compare(right, left);
}

bool time_compare(operand_list_t* left, operand_list_t* right)
{
	if (left->time == right->time) {
		return lexicographical_compare(left, right);
	}
	return (left->time < right->time);
}

bool no_sort(operand_list_t* left, operand_list_t* right)
{
	(void)left;
	(void)right;
	return false;
}
