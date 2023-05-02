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
#include "libkm/io/printf.h"
#include "libkm/string.h"

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

file_type get_file_type(struct stat statBuf, ls_flags flags)
{
	// Check the file type
	if (S_ISREG(statBuf.st_mode) || flags.display_directory_as_file) {
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

static void set_symlink_destination(ls_file* file)
{
	static const int bufferSize = PATH_MAX;
	char buffer[bufferSize + 1];

	ssize_t length = readlink(file->path, buffer, bufferSize);
	if (length < 0) {
		perror(file->path);
		return set_status(LS_MINOR_ERROR);
	}
	buffer[length] = '\0';
	file->symlinkDestination = km_strdup(buffer);
	if (file->symlinkDestination == NULL) {
		return set_status(LS_SERIOUS_ERROR);
	}
}

void set_stat_info(ls_file* file, ls_flags flags)
{
	struct stat statBuf;

	 // Get file status
	if (lstat(file->path, &statBuf) == -1)
	{
		perror(file->path);
		return set_status(LS_MINOR_ERROR);
	}
	file->statInfo = statBuf;

	file->type = get_file_type(statBuf, flags);
	if (file->type == symlink_type)
	{
		set_symlink_destination(file);
	}
	else if (file->type == UNKNOWN_TYPE)
	{
		return set_status(LS_SERIOUS_ERROR);
	}

	file->time = (flags.use_access_time) ? file->statInfo.st_atime : file->statInfo.st_mtime;
}

void get_files_in_directory(const char* dirName, ls_flags flags, km_vector_file* directory_files)
{
    DIR*			dir;
    struct dirent*	entry;

    dir = opendir(dirName);
    if (dir == NULL)
	{
        perror(dirName);
        return set_status(LS_MINOR_ERROR);
    }

	ls_status status = LS_SUCCESS;
    // Read each entry in directory
    while ((entry = readdir(dir)) != NULL && status == LS_SUCCESS)
	{
		if ((flags.hidden_directories) == false && entry->d_name[0] == '.') {
			continue ; // skip hidden directories
		}
		add_file(dirName, entry->d_name, directory_files);
		if (!status_success()) {
			break ;
		}
		set_stat_info(km_vector_file_back(directory_files), flags);
		if (get_status() == LS_MINOR_ERROR) {
			km_vector_file_pop_back(directory_files);
			set_status(LS_SUCCESS);
		}
	}
    closedir(dir);

	if (!status_success())
	{
		km_vector_file_destroy(directory_files);
	}
}
