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

#include "../parsing/parse.h"
#include "libkm.h"

#include <stdlib.h>

void clear_list(operand_list_t* node)
{
	operand_list_t* tmp;

	while (node != NULL)
	{
		tmp = node;
		node = node->next;
		tmp->next = NULL;
		free((void*)tmp->name);
		tmp->name = NULL;
		free(tmp);
	}
}

operand_list_t* list_append(operand_list_t** list, const char* operand)
{
	if (list == NULL){
		return NULL;
	}

	operand_list_t* newNode;
	newNode = malloc(sizeof(operand_list_t));

	if (newNode == NULL) {
		return NULL;
	}

	newNode->next = NULL;
	newNode->name = km_strdup(operand);
	if (newNode->name == NULL) {
		free(newNode);
		return NULL;
	}

	if (*list == NULL)
	{
		*list = newNode;
	}
	else
	{
		operand_list_t* node = *list;
		while (node->next != NULL) {
			node = node->next;
		}
		node->next = newNode;
	}
	newNode->time = 0;
	return newNode;
}

operand_list_t* list_append_node(operand_list_t** list, operand_list_t* newNode)
{
	newNode->next = NULL;
	if (*list == NULL)
	{
		*list = newNode;
	}
	else
	{
		operand_list_t* node = *list;
		while (node->next) {
			node = node->next;
		}
		node->next = newNode;
	}
	
	return newNode;
}
