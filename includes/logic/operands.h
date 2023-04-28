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
ls_status set_operand_data(km_vector_file* operands, ls_flags flags);

/*!
 * @brief splits operands into files and directories (by type)
 * @param operands -
 * @param files make sure it has not been initialised
 * @param directories make sure it has not been initialised 
 *
 * @NOTE: the operands vector will be destroyed after calling this function
*/
ls_status split_operands(km_vector_file* operands, km_vector_file* files, km_vector_file* directories);

/*!
 * @brief handles listing of all files etc.
 * @param files will print all information about file
 * @param directories all entries will be listed (might be recursively)
 * @param flags -
*/
ls_status list_operands(const km_vector_file* files, const km_vector_file* directories, ls_flags flags);
