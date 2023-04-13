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

bool set_stat_info(operand_list_t* operand, ls_flags flags)
{
	struct stat statBuf;

	 // Get file status
	if (stat(operand->path, &statBuf) == -1)
	{
		perror(operand->path);
		return false;
	}
	operand->statInfo = statBuf;

	operand->type = get_file_type(statBuf, flags);
	if (operand->type == UNKNOWN_TYPE)
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