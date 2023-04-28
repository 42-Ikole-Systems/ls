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
#include "utility/file.h"

#include <libkm/string.h>
#include <libkm/io/printf.h>

static ls_status set_directory(ls_file* file, const char* dir, const char* filename)
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
	file->path = path;
	return LS_SUCCESS;
}

static ls_status create_file(ls_file* file, const char* dir, const char* filename)
{
	ls_status status = LS_SUCCESS;

	file->symlinkDestination = NULL;
	file->time = 0;
	file->filename = km_strdup(filename);
	if (file->filename == NULL) {
		return LS_SERIOUS_ERROR;
	}
	status = set_directory(file, dir, filename);
	if (status != LS_SUCCESS)
	{
		free((void*)file->filename);
	}
	return status;
}

ls_status add_file(const char* dir, const char* operand, km_vector_file* operands)
{
	ls_file file;

	if (create_file(&file, dir, operand) != LS_SUCCESS)
	{
		return LS_SERIOUS_ERROR;
	}
	ls_file* newFile = km_vector_file_push_back(operands, file);
	if (newFile == NULL) {
		return LS_SERIOUS_ERROR;
	}
	newFile->type = UNKNOWN_TYPE;
	return LS_SUCCESS;
}

void erase_file_if_filename(km_vector_file* files, const char* filename)
{
	for (size_t i = 0; i < files->size; i++) {
		const ls_file* file = km_vector_file_at(files, i);
		if (km_strcmp(file->filename, filename) == 0) {
			km_vector_file_erase_position(files, i);
			break;
		}
	}
}

void destroy_file(ls_file* file)
{
	free((void*)file->filename);
	free((void*)file->path);
	free((void*)file->symlinkDestination);
	file->filename = NULL;
	file->path = NULL;
	file->symlinkDestination = NULL;
}
