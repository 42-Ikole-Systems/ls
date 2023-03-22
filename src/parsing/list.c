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

#include "parse.h"
#include <stdlib.h>

void clear_list(operand_list_t* node)
{
	operand_list_t* tmp;

	while (node != NULL)
	{
		tmp = node;
		node = node->next;
		tmp->next = NULL;
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
	newNode->name = operand;

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
	return newNode;
}
