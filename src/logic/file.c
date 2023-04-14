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

static int set_symlink_destination(operand_list_t* operand)
{
	static const int bufferSize = PATH_MAX + 1;
	char buffer[bufferSize];

	if (readlink(operand->path, buffer, bufferSize) < 0) {
		return LS_ERROR;
	}
	operand->symlinkDestination = km_strdup(buffer);
	if (operand->symlinkDestination == NULL) {
		return LS_ERROR;
	}

	return LS_SUCCESS;
}

bool set_stat_info(operand_list_t* operand, ls_flags flags)
{
	struct stat statBuf;

	 // Get file status
	if (lstat(operand->path, &statBuf) == -1)
	{
		perror(operand->path);
		return false;
	}
	operand->statInfo = statBuf;

	operand->type = get_file_type(statBuf, flags);
	if (operand->type == symlink_type) {
		if (set_symlink_destination(operand) != LS_SUCCESS) {
			return false;
		}
	}
	else if (operand->type == UNKNOWN_TYPE)
	{
		return false;
	}

	operand->time = (flags & flag_use_access_time) ? operand->statInfo.st_atime : operand->statInfo.st_mtime;
	return true;
}

operand_list_t* get_files_in_directory(const char* dirName, ls_flags flags)
{
	operand_list_t* directory_files = NULL;
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(dirName);
    if (dir == NULL)
	{
        perror(dirName);
        return NULL;
    }

	bool error = false;
    // Read each entry in directory
    while ((entry = readdir(dir)) != NULL)
	{
		if ((flags & flag_hidden_directories) == false && entry->d_name[0] == '.') {
			continue ; // skip hidden directories
		}
		operand_list_t* currentOperand = list_append(&directory_files, dirName, entry->d_name);
		if (currentOperand == NULL)
		{
			error = true;
			break ;
		}
		if (set_stat_info(currentOperand, flags) == false) {
			error = true;
			break ;
		}
	}
    closedir(dir);

	if (error == true)
	{
		clear_list(directory_files);
		return NULL;
	}

    return directory_files;
}