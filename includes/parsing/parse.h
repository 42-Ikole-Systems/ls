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
 * @return ls_status
*/
ls_status parse_flags(const char* token, ls_flags* flags);

/*!
 * @brief parses command line arguments
 *
 * @param argc amount of arguments
 * @param argv the arguments
 * @param flags it will store the flags found in this variable
 *
 * @return true on success, false on error
 * 
 * @note it will ignore the first argument since this should be the name of the executable.
*/
ls_status parse(int argc, const char** argv, ls_flags* flags, operand_list_t** operand_list);
