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

file_type	get_file_type(const char* filename)
{
	struct stat statBuf;

	 // Get file status
	if (stat(filename, &statBuf) == -1)
	{
		perror("stat");
		return UNKNOWN_TYPE;
	}

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
		km_dprintf(stderr, "Unable to determine filetype for: `%s`", filename);
		return UNKNOWN_TYPE;
	}
}

operand_list_t* get_files_in_directory(const char* dir)
{
	operand_list_t* directory_files = NULL;
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(dir);
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
		currentOperand->type = get_file_type(filename);
		if (currentOperand->type == UNKNOWN_TYPE)
		{
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