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

file_type get_file_type(struct stat statBuf)
{
	// Check the file type
	if (S_ISREG(statBuf.st_mode)) {
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

bool set_stat_info(operand_list_t* operand)
{
	struct stat statBuf;

	 // Get file status
	if (stat(operand->name, &statBuf) == -1)
	{
		perror("stat");
		return false;
	}
	operand->statInfo = statBuf;

	operand->type = get_file_type(statBuf);
	if (operand->type == UNKNOWN_TYPE)
	{
		return false;
	}
	return true;
}

operand_list_t* get_files_in_directory(const char* dirName)
{
	operand_list_t* directory_files = NULL;
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(dirName);
    if (dir == NULL)
	{
        perror("opendir");
        return NULL;
    }

	bool error = false;
    // Read each entry in directory
    while ((entry = readdir(dir)) != NULL)
	{
		const char* filename = entry->d_name;
		operand_list_t* currentOperand = list_append(&directory_files, filename);
		if (currentOperand == NULL)
		{
			error = true;
			break ;
		}
		if (set_stat_info(currentOperand) == false) {
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