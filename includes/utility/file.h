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

#include "ft_ls.h"

/*!
 * @brief creats a ls_file and appends it to files
 * @param dir -
 * @param filename - 
 * @param files -
 * @return LS_SUCCESS or LS_SERIOUS_ERROR
*/
ls_status add_file(const char* dir, const char* filename, km_vector_file* files);

/*!
 * @brief erses file that matches filename from files
 * @param files -
 * @param filename -
*/
void erase_file_if_filename(km_vector_file* files, const char* filename);

/*!
 * @brief deallocates and clears all data contained in the file
*/
void destroy_file(ls_file* file);
