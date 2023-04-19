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
#include "libkm.h"
#include "utility/list.h"

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

file_type get_file_type(struct stat statBuf, ls_flags flags)
{
	// Check the file type
	if (S_ISREG(statBuf.st_mode) || flags & flag_display_directory_as_file) {
		return regular_file_type;
	}
	else if (S_ISDIR(statBuf.st_mode)) {
		return directory_type;
	}
	else if (S_ISLNK(statBuf.st_mode)) {
		return symlink_type;
	}
	else if (S_ISBLK(statBuf.st_mode)) {
		return block_device_type;
	}
	else if (S_ISCHR(statBuf.st_mode)) {
		return character_device_type;
	}
	else if (S_ISFIFO(statBuf.st_mode)) {
		return fifo_type;
	}
	else if (S_ISSOCK(statBuf.st_mode)) {
		return unix_socket_type;
	}
	else {
		km_dprintf(STDERR_FILENO, "Unable to determine filetype");
		return UNKNOWN_TYPE;
	}
}

static ls_status set_symlink_destination(operand_list_t* operand)
{
	static const int bufferSize = PATH_MAX;
	char buffer[bufferSize + 1];

	ssize_t length = readlink(operand->path, buffer, bufferSize);
	if (length < 0) {
		perror(operand->path);
		return LS_MINOR_ERROR;
	}
	buffer[length] = '\0';
	operand->symlinkDestination = km_strdup(buffer);
	if (operand->symlinkDestination == NULL) {
		return LS_SERIOUS_ERROR;
	}

	return LS_SUCCESS;
}

ls_status set_stat_info(operand_list_t* operand, ls_flags flags)
{
	struct stat statBuf;

	 // Get file status
	if (lstat(operand->path, &statBuf) == -1)
	{
		perror(operand->path);
		return LS_MINOR_ERROR;
	}
	operand->statInfo = statBuf;

	ls_status status = LS_SUCCESS;
	operand->type = get_file_type(statBuf, flags);
	if (operand->type == symlink_type)
	{
		status = set_symlink_destination(operand);
	}
	else if (operand->type == UNKNOWN_TYPE)
	{
		return LS_SERIOUS_ERROR;
	}

	operand->time = (flags & flag_use_access_time) ? operand->statInfo.st_atime : operand->statInfo.st_mtime;
	return status;
}

ls_status get_files_in_directory(const char* dirName, ls_flags flags, operand_list_t** directory_files)
{
	*directory_files = NULL;
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(dirName);
    if (dir == NULL)
	{
        perror(dirName);
        return LS_MINOR_ERROR;
    }

	ls_status status = LS_SUCCESS;
    // Read each entry in directory
    while ((entry = readdir(dir)) != NULL && status == LS_SUCCESS)
	{
		if ((flags & flag_hidden_directories) == false && entry->d_name[0] == '.') {
			continue ; // skip hidden directories
		}
		operand_list_t* currentOperand = list_append(directory_files, dirName, entry->d_name);
		if (currentOperand == NULL)
		{
			status = LS_SERIOUS_ERROR;
			break ;
		}
		status = set_stat_info(currentOperand, flags);
		if (status == LS_MINOR_ERROR) {
			list_remove_if(directory_files, entry->d_name);
			status = LS_SUCCESS;
		}
	}
    closedir(dir);

	if (status != LS_SUCCESS)
	{
		clear_list(*directory_files);
		*directory_files = NULL;
	}

	return status;
}