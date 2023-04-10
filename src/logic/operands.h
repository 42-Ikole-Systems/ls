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
 * @brief sets all data for operands
 * @param operands -
 * @param flags -
 * @return LS_SUCCESS on success, anything else on error
*/
int set_operand_data(operand_list_t* operands, ls_flags flags);

/*!
 * @brief splits operands into files and directories (by type)
 * @param operands -
 * @param files make sure it is empty and non NULL
 * @param directories make sure it is empty and non NULL 
 * 
 * NOTE: operands will be NULL after calling this
*/
void split_operands(operand_list_t** operands, operand_list_t** files, operand_list_t** directories);
