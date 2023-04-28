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
 * @brief sets statInfo in file and sets fileType
 * @param file -
 * @param flags -
 * @return -
*/
ls_status set_stat_info(ls_file* file, ls_flags flags);

/*!
 * @brief Gets all files in directory and sets their type
 * @param dirName -
 * @param flags -
 * @param directory_files it will store all the files in the directory here
 * @return directory files will be empty on error
*/
ls_status get_files_in_directory(const char* dirName, ls_flags flags, km_vector_file* directory_files);
