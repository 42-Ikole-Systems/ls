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
*/
void parse_flags(const char* token, ls_flags* flags);

/*!
 * @brief parses command line arguments
 *
 * @param argc amount of arguments
 * @param argv the arguments
 * @param flags it will store the flags found in this variable
 *
 * @note it will ignore the first argument since this should be the name of the executable.
*/
void parse(int argc, const char** argv, ls_flags* flags, km_vector_file* operands);
