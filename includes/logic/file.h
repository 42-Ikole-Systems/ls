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
 * @brief sets statInfo in operand and sets fileType
 * @param operand -
 * @return -
*/
ls_status set_stat_info(operand_list_t* operand, ls_flags flags);

/*!
 * @brief Gets all files in directory and sets their type
 * @param dirName -
 * @param flags -
 * @param directory_files it will store all the files in the directory here
 * @return directory files will be empty on error
*/
ls_status get_files_in_directory(const char* dirName, ls_flags flags, operand_list_t** directory_files);
