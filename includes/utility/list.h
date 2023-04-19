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
* @brief deallocates and cleans up the list
*
* @param list -
*/
void clear_list(operand_list_t* list);

/*!
* @brief creates and appends a new node to the end of the list
*
* @param list pointer to the list
* @param dir will be copied
* @param filename will be copied
* 
* @return newly apended node, NULL if fails
*/
operand_list_t* list_append(operand_list_t** list, const char* dir, const char* filename);

/*!
	* @brief removes node if filename == filename
	* @param filename node with this filename will be removed
*/
void list_remove_if(operand_list_t** list, const char* filename);

/*!
	* @brief appends node to end of list
	* @param list pointer to the list
	* @param newNode node to append
	* @return newNode
*/
operand_list_t* list_append_node(operand_list_t** list, operand_list_t* newNode);