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
 * @param filename
 * @return UNKNOWN_TYPE on failure
*/
file_type get_file_type(struct stat statBuf);

/*!
 * @brief Gets all files in directory and sets their type
 * @param dir -
 * @return -
*/
operand_list_t* get_files_in_directory(const char* dir, ls_flags flags);

/*!
 * @brief sets statInfo in operand and sets fileType
 * @param operand -
 * @return true on success, false on failure
*/
bool set_stat_info(operand_list_t* operand, ls_flags flags);
