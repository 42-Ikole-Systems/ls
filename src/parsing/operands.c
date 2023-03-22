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

	if (S_ISREG(statBuffer.st_mode)) {
		newNode->type = file_type;
    }
	else if (S_ISDIR(statBuffer.st_mode)) {
		newNode->type = directory_type;
    }
	else if (S_ISLNK(statBuffer.st_mode)) {
		newNode->type = symlink_type;
    }
	else {
        km_printf("Unable to classify %s\n", operand);
		return LS_PARSE_ERROR;
    }
	return 1;
}
