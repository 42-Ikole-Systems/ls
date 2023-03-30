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

#include <libkm.h>

#include "parse.h"
#include <sys/stat.h>
#include <stdio.h>

int parse_operand(const char* operand, operand_list_t** operand_list)
{
	struct stat statBuffer;
	if (lstat(operand, &statBuffer) == -1)
	{
		perror(operand);
        return LS_PARSE_ERROR;
    }

	operand_list_t* newNode = list_append(operand_list, operand);
	newNode->type = UNKNOWN_TYPE;
	return 0;
}
