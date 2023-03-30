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
 * @brief -
 * 
 * @param token to check
 * @param flags variable to store found flags
 * 
 * @return the amount of flags found, -1 on error
*/
int parse_flags(const char* token, ls_flags* flags);

/*!
 * @brief parses operands
 *
 * @param operand -
 * @param directoryOperands -
 * @param nonDirectoryOperands -
*/
int parse_operand(const char* operand, operand_list_t** operand_list);
