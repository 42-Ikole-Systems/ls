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
#include "file.h"
#include "sort.h"
#include "libkm.h"
#include "colorcodes.h"

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

ls_status set_operand_data(operand_list_t* operands, ls_flags flags)
{
	for (operand_list_t* node = operands; node != NULL; node = node->next)
	{
		ls_status status = set_stat_info(node, flags);
		if (status != LS_SUCCESS)
		{
			clear_list(operands);
			return status;
		}
	}
	return LS_SUCCESS;
}

void split_operands(operand_list_t** operands, operand_list_t** files, operand_list_t** directories)
{
	operand_list_t* node = *operands;
	while (node != NULL)
	{
		operand_list_t* tmp = node;
		node = node->next;
		if (tmp->type == directory_type) {
			list_append_node(directories, tmp);
		}
		else {
			list_append_node(files, tmp);
		}
	}
	*operands = NULL;
}

static char get_entry_type(const operand_list_t* node)
{
	switch (node->type)
	{
		case regular_file_type: return '-';
		case directory_type: return 'd';
		case symlink_type: return 'l';
		case block_device_type: return 'b';
		case character_device_type: return 'c';
		case fifo_type: return 'p';
		case unix_socket_type: return 's';
		default:
			assert(node->type == UNKNOWN_TYPE);
			return '?';
	}
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_file_mode(const operand_list_t* node)
{
	static char permissionString[10];
	int filemode = node->statInfo.st_mode;

	// owner permisison
	permissionString[0] = (filemode & S_IRUSR) ? 'r' : '-';
	permissionString[1] = (filemode & S_IWUSR) ? 'w' : '-';
	if (filemode & S_IXUSR && filemode & S_ISUID) {
		permissionString[2] = 's';
	} else if (filemode & S_IXUSR) {
		permissionString[2] = 'x';
	} else if (filemode & S_ISUID) {
		permissionString[2] = 'S';
	} else {
		permissionString[2] = '-';
	}
	
	// group permission
	permissionString[3] = (filemode & S_IRGRP) ? 'r' : '-';
	permissionString[4] = (filemode & S_IWGRP) ? 'w' : '-';
	if (filemode & S_IXGRP && filemode & S_ISGID) {
		permissionString[5] = 's';
	} else if (filemode & S_IXGRP) {
		permissionString[5] = 'x';
	} else if (filemode & S_ISGID) {
		permissionString[5] = 'S';
	} else {
		permissionString[5] = '-';
	}
	
	// others permisison
	permissionString[6] = (filemode & S_IROTH) ? 'r' : '-';
	permissionString[7] = (filemode & S_IWOTH) ? 'w' : '-';
	if (filemode & S_IXOTH && filemode & S_ISVTX) {
		permissionString[8] = 't';
	} else if (filemode & S_IXOTH) {
		permissionString[8] = 'x';
	} else if (filemode & S_ISVTX) {
		permissionString[8] = 'T';
	} else {
		permissionString[8] = '-';
	}

	permissionString[9] = '\0';
	return permissionString;
}

static size_t get_hard_links(const operand_list_t* node)
{
	return (size_t)node->statInfo.st_nlink;
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_owner_name(const operand_list_t* node)
{
	struct passwd* pw = getpwuid(node->statInfo.st_uid);
	if (pw == NULL) {
		return NULL;
	}
	return pw->pw_name;
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_group_name(const operand_list_t* node)
{
	struct group* gr = getgrgid(node->statInfo.st_gid);
	if (gr == NULL) {
		return NULL;
	}
	return gr->gr_name;
}

static size_t get_filesize(const operand_list_t* node)
{
	return (size_t)(node->statInfo.st_size);
}

static char* get_time(const operand_list_t* node)
{
	char* modifiedTimeString = ctime(&node->time); // DO NOT FREE
	if (modifiedTimeString == NULL) {
		return NULL;
	}

	const char* dayName = km_strtok(modifiedTimeString, " ");
	const char* month = km_strtok(NULL, " ");
	const char* dayNumber = km_strtok(NULL, " ");
	const char* timeString = km_strtok(NULL, " ");
	const char* year = km_strtok(NULL, "\n");
	(void)dayName;

	const time_t currentTime = time(NULL);
	const time_t elevenMonths = 11 * 30 * 24 * 60 * 60; // ~ not very precise but good enough
	const char* yearTime = (node->time < currentTime - elevenMonths) ? year : timeString;

	char* result = NULL;
	if (km_sprintf(&result , "%.2s %s %.5s", dayNumber, month, yearTime) < 0) {
		free(result);
		return NULL;
	}

	return (result);
}

static size_t get_most_links(const operand_list_t* files)
{
	off_t links = 0;
	for (const operand_list_t* node = files; node != NULL; node = node->next)
	{
		if (node->statInfo.st_nlink > links) {
			links = node->statInfo.st_nlink;
		}
	}
	return (size_t)links;
}

static size_t get_largest_file_size(const operand_list_t* files)
{
	off_t largestFileSize = 0;
	for (const operand_list_t* node = files; node != NULL; node = node->next)
	{
		if (node->statInfo.st_size > largestFileSize) {
			largestFileSize = node->statInfo.st_size;
		}
	}
	return (size_t)largestFileSize;
}

static int get_amount_of_characters(size_t n)
{
	if (n == 0) {
		return 1;
	}

	int amountOfCharacers = 0;
	for (; n != 0; n /= 10) {
		amountOfCharacers++;
	}
	return amountOfCharacers;
}

static void remove_directory_indicators(operand_list_t** directories)
{
	list_remove_if(directories, ".");
	list_remove_if(directories, "..");
}

static bool should_display_directory_name(const operand_list_t* files, const operand_list_t* directories, size_t depth)
{
	if (depth > 0) {
		return true;
	}
	else if (files && directories) {
		return true;
	}
	else if (!files && directories && directories->next) {
		return true;
	}
	else {
		return false;
	}
}

static bool is_executable(const operand_list_t* file)
{
	const int filemode = file->statInfo.st_mode;
	return (filemode & S_IXUSR || filemode & S_IXGRP || filemode & S_IXOTH);
}


static char* get_filename(const operand_list_t* file, ls_flags flags)
{
	char* filename = NULL;
	if (flags & flag_colorised_output)
	{
		char* color = "";
		switch (file->type)
		{
			case regular_file_type: {
				if (is_executable(file)) {
					color = COLOR_RED;
				}
				break ;
			}
			case directory_type: {
				color = COLOR_LIGHT_BLUE;
				break ;
			}
			case symlink_type: {
				color = COLOR_PURPLE;
				break ;
			}
			default: {
				// do nothing
			}
		}
		if (km_sprintf(&filename, "%s%s%s", color, file->filename, COLOR_RESET) < 0) {
			filename = NULL;
		}
	}
	else {
		filename = km_strdup(file->filename);
	}

	if (filename == NULL) {
		return NULL;
	}

	if (file->type == symlink_type)
	{
		char* filenameWithDestination;
		if (km_sprintf(&filenameWithDestination, "%s -> %s", filename, file->symlinkDestination) < 0) {
			filenameWithDestination = NULL;
		}
		free(filename);
		return filenameWithDestination;
	}
	else {
		return filename;
	}
}

static ls_status print_files(const operand_list_t* files, ls_flags flags)
{
	ls_status status = LS_SUCCESS;

	const int hardlinkPrecision = get_amount_of_characters(get_most_links(files)) + 1;
	const int filesizePrecision = get_amount_of_characters(get_largest_file_size(files)) + 1;

	// files
	for (const operand_list_t* node = files; status == LS_SUCCESS && node != NULL; node = node->next)
	{
		if (flags & flag_long_format)
		{
			const char entryType = get_entry_type(node); // entry type
			const char* fileMode = get_file_mode(node); // File mode (permissions)
			const size_t hardLinks = get_hard_links(node); // Number of hard links
			const char* ownerName = get_owner_name(node); // Owner name (or ID)
			const char* groupName = get_group_name(node); // Group name (or ID)
			const size_t filesize = get_filesize(node); // File size (in bytes)
			const char* fileTime = get_time(node); // Date and time of last modification !! MUST BE FREED !!
			const char* filename = get_filename(node, flags); // !! MUST BE FREEED !!

			if (ownerName == NULL || groupName == NULL || fileTime == NULL || filename == NULL)
			{
				status = LS_SERIOUS_ERROR;
			}

			static const int fileTimeWidth = 12; // ie: `12 Apr 13:15` || ` 12 Apr 2022`
			if (status == LS_SUCCESS &&
				km_printf("%c%s %*llu %s %s %*llu %*s %s\n",
					entryType, // %c
					fileMode, // %s
					hardlinkPrecision, // %*
					hardLinks, // llu
					ownerName, // %s
					groupName, // %s
					filesizePrecision, //*
					filesize, // %llu
					fileTimeWidth, // %*
					fileTime, // %s
					filename // %s
				) < 0)
			{
				status = LS_SERIOUS_ERROR;
			}

			free((void*)filename);
			free((void*)fileTime);
		}
		else
		{
			const char whitespace = (node->next != NULL) ? '\t' : '\n';
			if (km_printf("%s%c", node->filename, whitespace) < 0) {
				status = LS_SERIOUS_ERROR;
			}
		}
	}
	return status;
}

static ls_status list_subdirectories(operand_list_t** directoryEntries, ls_flags flags, size_t depth)
{
	operand_list_t* subdirFiles = NULL;
	operand_list_t* subdirDirs = NULL;
	split_operands(directoryEntries, &subdirFiles, &subdirDirs);
	remove_directory_indicators(&subdirDirs);

	// recursively handle each subdirectory
	ls_status status = list_operands(NULL, subdirDirs, flags, depth + 1);

	clear_list(subdirFiles);
	clear_list(subdirDirs);

	return status;
}

static ls_status list_total_blocks(const operand_list_t* entries)
{
	size_t total_blocks = 0;
	for (const operand_list_t* node = entries; node != NULL; node = node->next)
	{
		total_blocks += node->statInfo.st_blocks;
	}
	if (km_printf("total %llu\n", total_blocks) < 0) {
		return LS_SERIOUS_ERROR;
	}
	return LS_SUCCESS;
}

static ls_status list_directories(const operand_list_t* directories, ls_flags flags, bool displayDirectoryName, size_t depth)
{
	ls_status status = LS_SUCCESS;
	// directory
	for (const operand_list_t* node = directories; status == LS_SUCCESS && node != NULL; node = node->next)
	{
		if (displayDirectoryName)
		{
			if (km_printf("\n%s:\n", node->path) < 0) {
				return LS_SERIOUS_ERROR;
			}
		}

		operand_list_t* directoryEntries = NULL;
		status = get_files_in_directory(node->path, flags, &directoryEntries);
		
		if (status == LS_SUCCESS) {
			sort(&directoryEntries, flags);
			status = list_total_blocks(directoryEntries);
		}

		if (status == LS_SUCCESS) {
			status = print_files(directoryEntries, flags);
		}

		if (status == LS_SUCCESS && flags & flag_recursive) {
			status = list_subdirectories(&directoryEntries, flags, depth);
		}
		clear_list(directoryEntries);
	}
	return status;
}

ls_status list_operands(const operand_list_t* files, const operand_list_t* directories, ls_flags flags, size_t depth)
{
	ls_status status = LS_SUCCESS;

	status = print_files(files, flags);
	if (status == LS_SUCCESS) {
		bool displayDirectoryName = should_display_directory_name(files, directories, depth);
		status = list_directories(directories, flags, displayDirectoryName, depth);
	}
	return status;
}
