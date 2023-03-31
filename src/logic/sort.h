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
void sort(operand_list_t** list, ls_flags flags);

/*!
 * @brief prototype for compare function
 * @return true if need to be swapped, false otherwise
*/
typedef bool(*compare_function_t)(operand_list_t* left, operand_list_t* right);

/*!
 * @brief get the function used for comparing
*/
compare_function_t get_compare_function(ls_flags flags);


///////////////////////
// Compare functions //
///////////////////////


bool lexicographical_compare(operand_list_t* left, operand_list_t* right);
bool reverse_lexicographical_compare(operand_list_t* left, operand_list_t* right);
bool time_modified_compare(operand_list_t* left, operand_list_t* right);
bool time_access_compare(operand_list_t* left, operand_list_t* right);
bool no_sort(operand_list_t* left, operand_list_t* right);
