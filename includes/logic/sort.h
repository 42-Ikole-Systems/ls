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

#pragma once

#include "ft_ls.h"

/*!
 * @brief sorts list based on what flags are set
*/
void sort(km_vector_file* files, ls_flags flags);

/*!
 * @brief prototype for compare function
 * @return true if need to be swapped, false otherwise
*/
typedef bool(*compare_function_t)(const ls_file* left, const ls_file* right);

/*!
 * @brief get the function used for comparing
*/
compare_function_t get_compare_function(ls_flags flags);

///////////////////////
// Compare functions //
///////////////////////

bool lexicographical_compare(const ls_file* left, const ls_file* right);
bool reverse_lexicographical_compare(const ls_file* left, const ls_file* right);
bool time_compare(const ls_file* left, const ls_file* right);
bool no_sort(const ls_file* left, const ls_file* right);
