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

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

static int set_operand_types(operand_list_t* operands, ls_flags flags)
{
	for (operand_list_t* node = operands; node != NULL; node = node->next)
	{
		if (set_stat_info(node, flags) == false)
		{
			clear_list(operands);
			return LS_ERROR;
		}
	}
	return true;
}

/*!
 * @brief splits operands into files and directories (by type)
 * @param operands -
 * @param files make sure it is empty and non NULL
 * @param directories make sure it is empty and non NULL 
 * 
 * NOTE: operands will be NULL after calling this
*/
static void split_operands(operand_list_t** operands, operand_list_t** files, operand_list_t** directories)
{
	for (operand_list_t* node = *operands; node != NULL; node = node->next)
	{
		if (node->type == directory_type) {
			list_append_node(directories, node);
		}
		else {
			list_append_node(files, node);
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

static size_t get_most_links(const operand_list_t* files, const operand_list_t* directories)
{
	off_t links = 0;
	for (const operand_list_t* node = files; node != NULL; node = node->next)
	{
		if (node->statInfo.st_nlink > links) {
			links = node->statInfo.st_nlink;
		}
	}
	for (const operand_list_t* node = directories; node != NULL; node = node->next)
	{
		if (node->statInfo.st_nlink > links) {
			links = node->statInfo.st_nlink;
		}
	}
	return (size_t)links;
}

static size_t get_largest_file_size(const operand_list_t* files, const operand_list_t* directories)
{
	off_t largestFileSize = 0;
	for (const operand_list_t* node = files; node != NULL; node = node->next)
	{
		if (node->statInfo.st_size > largestFileSize) {
			largestFileSize = node->statInfo.st_size;
		}
	}
	for (const operand_list_t* node = directories; node != NULL; node = node->next)
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

static int print_operands(const operand_list_t* files, const operand_list_t* directories, ls_flags flags)
{
	int status = LS_SUCCESS;

	const int hardlinkPrecision = get_amount_of_characters(get_most_links(files, directories)) + 1;
	const int filesizePrecision = get_amount_of_characters(get_largest_file_size(files, directories)) + 1;
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
			const char* fileTime = get_time(node); // Date and time of last modification
			const char* filename = node->name; // File name

			if (ownerName == NULL || groupName == NULL || fileTime == NULL)
			{
				status = LS_ERROR;
			}

			if (status == LS_SUCCESS
				&& km_printf("%c%s % .*llu %s %s % .*llu %s %s\n", entryType, fileMode, hardlinkPrecision, hardLinks, ownerName, groupName, filesizePrecision, filesize, fileTime, filename) < 0)
			{
				status = LS_ERROR;
			}

			free((void*)fileTime);
		}
		else
		{
			if (km_printf("%s\t", node->name) < 0) {
				status = LS_ERROR;
			}
		}
	}
	// directory
	for (const operand_list_t* node = directories; status == LS_SUCCESS && node != NULL; node = node->next)
	{
		km_printf("directory: %s\n", directories->name);
		// get everything in directory,
		// if recurse is on go back to list operands with subdirectories..
	}
	return status;
}

int list_operands(operand_list_t* operands, ls_flags flags)
{
	if (set_operand_types(operands, flags) == LS_ERROR) {
		return LS_ERROR;
	}

	operand_list_t* files = NULL;
	operand_list_t* directories = NULL;
	sort(&operands, flags);
	split_operands(&operands, &files, &directories);

	print_operands(files, directories, flags);
	clear_list(files);
	clear_list(directories);
	return 0;
}
