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
#include "libkm/string.h"

#include <stdlib.h>

void sort(km_vector_file* files, ls_flags flags)
{
    bool swapped;
	ls_file* arr = files->arr;

	compare_function_t compare = get_compare_function(flags);

    do {
        swapped = false;
		for (size_t i = 0; i + 1 < files->size; i++)
		{
			if (compare(&(arr[i]), &(arr[i + 1])) == true)
			{
				ls_file tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = true;
			}
		}
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


bool lexicographical_compare(const ls_file* left, const ls_file* right)
{
	return (km_strcmp(left->filename, right->filename) > 0);
}

bool reverse_lexicographical_compare(const ls_file* left, const ls_file* right)
{
	return lexicographical_compare(right, left);
}

bool time_compare(const ls_file* left, const ls_file* right)
{
	if (left->time == right->time) {
		return lexicographical_compare(left, right);
	}
	return (left->time < right->time);
}

bool no_sort(const ls_file* left, const ls_file* right)
{
	(void)left;
	(void)right;
	return false;
}
