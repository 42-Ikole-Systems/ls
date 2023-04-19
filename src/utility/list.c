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

#include "parsing/parse.h"
#include "libkm.h"

#include <stdlib.h>

static void free_node(operand_list_t* node)
{
	node->next = NULL;
	free((void*)node->filename);
	free((void*)node->path);
	free((void*)node->symlinkDestination);
	node->filename = NULL;
	node->path = NULL;
	free(node);
}

void list_remove_if(operand_list_t** list, const char* filename)
{
	operand_list_t* prev = NULL;
	for (operand_list_t* node = *list; node != NULL; node = node->next)
	{
		if (km_strcmp(node->filename, filename) == 0)
		{
			if (prev == NULL) {
				*list = node->next;
			}
			else {
				prev->next = node->next;
			}
			free_node(node);
			return ;
		}
		prev = node;
	}
}

void clear_list(operand_list_t* node)
{
	operand_list_t* tmp;

	while (node != NULL)
	{
		tmp = node;
		node = node->next;
		free_node(tmp);
	}
}

static ls_status set_directory(operand_list_t* node, const char* dir, const char* filename)
{
	char* path = NULL;

	if (dir == NULL)
	{
		path = km_strdup(filename);
		if (path == NULL)
		{
			return LS_SERIOUS_ERROR;
		}
	}
	else
	{
		if (km_sprintf(&path, "%s/%s", dir, filename) < 0)
		{
			return LS_SERIOUS_ERROR;
		}
	}
	node->path = path;
	return LS_SUCCESS;
}

operand_list_t* list_append(operand_list_t** list, const char* dir, const char* filename)
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
	newNode->filename = km_strdup(filename);
	if (newNode->filename == NULL)
	{
		free(newNode);
		return NULL;
	}
	
	if (set_directory(newNode, dir, filename) != LS_SUCCESS)
	{
		free((void*)newNode->filename);
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
	newNode->symlinkDestination = NULL;
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
