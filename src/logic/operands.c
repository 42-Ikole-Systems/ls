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
#include "logic/file.h"
#include "logic/sort.h"
#include "logic/operands.h"
#include "utility/file.h"

#include <libkm/io/printf.h>
#include <libkm/colorcodes.h>
#include <libkm/string.h>

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

void set_operand_data(km_vector_file* operands, ls_flags flags)
{
	for (size_t i = 0; i < operands->size;)
	{
		ls_file* file = km_vector_file_at(operands, i);

		set_stat_info(file, flags);
		if (get_status() == LS_MINOR_ERROR)
		{
			km_vector_file_erase_position(operands, i);
			set_status(LS_SUCCESS);
			continue ;
		}
		if (!status_success())
		{
			km_vector_file_destroy(operands);
			return ;
		}
		i++;
	}
	if (km_vector_file_empty(operands)) {
		set_status(LS_MINOR_ERROR);
	}
}

void split_operands(km_vector_file* operands, km_vector_file* files, km_vector_file* directories)
{
	km_vector_file_initialise(files, operands->destroy_element, operands->deep_copy);
	if (km_vector_file_reserve(files, operands->size) == false) {
		km_vector_file_destroy(operands);
		return set_status(LS_SERIOUS_ERROR);
	}
	km_vector_file_initialise(directories, operands->destroy_element, operands->deep_copy);
	if (km_vector_file_reserve(directories, operands->size) == false) {
		km_vector_file_destroy(files);
		km_vector_file_destroy(operands);
		return set_status(LS_SERIOUS_ERROR);
	}
	
	for (size_t i = 0; i < operands->size; i++)
	{
		const ls_file* file = km_vector_file_at(operands, i); 
		if (file->type == directory_type) {
			km_vector_file_push_back(directories, *file);
		}
		else {
			km_vector_file_push_back(files, *file);
		}
	}
	// since we did a shallow copy we do not want to destroy all the elements but just clear the vector
	operands->destroy_element = NULL; // dont do this at home kids
	km_vector_file_destroy(operands);
	km_vector_file_initialise(operands, files->destroy_element, files->deep_copy); // bring it back to a working state
}

static char get_entry_type(const ls_file* file)
{
	switch (file->type)
	{
		case regular_file_type: return '-';
		case directory_type: return 'd';
		case symlink_type: return 'l';
		case block_device_type: return 'b';
		case character_device_type: return 'c';
		case fifo_type: return 'p';
		case unix_socket_type: return 's';
		default:
			assert(file->type == UNKNOWN_TYPE);
			return '?';
	}
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_file_mode(const ls_file* file)
{
	static char permissionString[10];
	int filemode = file->statInfo.st_mode;

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

static size_t get_hard_links(const ls_file* file)
{
	return (size_t)file->statInfo.st_nlink;
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_owner_name(const ls_file* file, ls_flags flags)
{
	if (flags.display_groupname) {
		return "";
	}

	struct passwd* pw = getpwuid(file->statInfo.st_uid);
	if (pw == NULL) {
		return NULL;
	}
	return pw->pw_name;
}

/*!
 * @NOTE: DO NOT FREE returnvalue (static memory)
*/
static const char* get_group_name(const ls_file* file)
{
	struct group* gr = getgrgid(file->statInfo.st_gid);
	if (gr == NULL) {
		return NULL;
	}
	return gr->gr_name;
}

static size_t get_filesize(const ls_file* file)
{
	return (size_t)(file->statInfo.st_size);
}

static char* get_time(const ls_file* file)
{
	char* modifiedTimeString = ctime(&file->time); // DO NOT FREE
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
	const char* yearTime = (file->time < currentTime - elevenMonths) ? year : timeString;

	char* result = NULL;
	if (km_sprintf(&result , "%s %2s %.5s", month, dayNumber, yearTime) < 0) {
		return NULL;
	}

	return (result);
}

static size_t get_most_links(const km_vector_file* files)
{
	off_t links = 0;
	for (size_t i = 0; i < files->size; i++)
	{
		const ls_file* file = &(files->arr[i]);
		if (file->statInfo.st_nlink > links) {
			links = file->statInfo.st_nlink;
		}
	}
	return (size_t)links;
}

static size_t get_largest_file_size(const km_vector_file* files)
{
	off_t largestFileSize = 0;
	for (size_t i = 0; i < files->size; i++)
	{
		const ls_file* file = &(files->arr[i]);
		if (file->statInfo.st_size > largestFileSize) {
			largestFileSize = file->statInfo.st_size;
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

static void remove_directory_indicators(km_vector_file* directories)
{
	erase_file_if_filename(directories, ".");
	erase_file_if_filename(directories, "..");
}

static bool is_executable(const ls_file* file)
{
	const int filemode = file->statInfo.st_mode;
	return (filemode & S_IXUSR || filemode & S_IXGRP || filemode & S_IXOTH);
}

static char* get_filename(const ls_file* file, ls_flags flags)
{
	char* filename = NULL;
	if (flags.colorised_output)
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

/*!
 * @brief keeps track of how many files were printed, returns files listed from before call;
 * @param amount the amount of files printed
 * @return the amount of files printed
*/
static size_t files_printed(size_t amount)
{
	static size_t filesListed = 0;
	size_t oldVal = filesListed;
	filesListed += amount;

	return oldVal;
}

static void add_file_printed()
{
	files_printed(1);
}

static void print_files(const km_vector_file* files, ls_flags flags)
{
	const int hardlinkWidth = get_amount_of_characters(get_most_links(files)) + 1;
	const int filesizeWidth = get_amount_of_characters(get_largest_file_size(files)) + 1;

	for (size_t i = 0; status_success() && i < files->size; i++)
	{
		const ls_file* file = &(files->arr[i]);
		const char* filename = get_filename(file, flags); // !! MUST BE FREEED !!
		if (flags.long_format)
		{
			const char entryType = get_entry_type(file); // entry type
			const char* fileMode = get_file_mode(file); // File mode (permissions)
			const size_t hardLinks = get_hard_links(file); // Number of hard links
			const char* ownerName = get_owner_name(file, flags); // Owner name (or ID)
			const char* groupName = get_group_name(file); // Group name (or ID)
			const size_t filesize = get_filesize(file); // File size (in bytes)
			const char* fileTime = get_time(file); // Date and time of last modification !! MUST BE FREED !!

			if (ownerName == NULL || groupName == NULL || fileTime == NULL || filename == NULL)
			{
				set_status(LS_SERIOUS_ERROR);
			}

			static const int fileTimeWidth = 12; // ie: `12 Apr 13:15` || ` 12 Apr 2022`
			if (status_success() &&
				km_printf("%c%s %*llu %s %s %*llu %*s %s\n",
					entryType, // %c
					fileMode, // %s
					hardlinkWidth, // %*
					hardLinks, // llu
					ownerName, // %s
					groupName, // %s
					filesizeWidth, //*
					filesize, // %llu
					fileTimeWidth, // %*
					fileTime, // %s
					filename // %s
				) < 0)
			{
				set_status(LS_SERIOUS_ERROR);
			}
			add_file_printed();
			free((void*)fileTime);
		}
		else
		{
			const char whitespace = (i + 1 == files->size) ? '\n' : '\t';
			if (km_printf("%s%c", filename, whitespace) < 0) {
				set_status(LS_SERIOUS_ERROR);
			}
		}
		free((void*)filename);
	}
}

static void list_subdirectories(km_vector_file* directoryEntries, ls_flags flags)
{
	km_vector_file subdirFiles;
	km_vector_file subdirDirs;
	split_operands(directoryEntries, &subdirFiles, &subdirDirs);
	if (!status_success()) {
		return ;
	}
	remove_directory_indicators(&subdirDirs);

	// recursively handle each subdirectory
	list_operands(NULL, &subdirDirs, flags);

	km_vector_file_destroy(&subdirFiles);
	km_vector_file_destroy(&subdirDirs);
}

static void list_total_blocks(const km_vector_file* entries)
{
	size_t total_blocks = 0;
	for (size_t i = 0; i < entries->size; i++)
	{
		const ls_file* file = &(entries->arr[i]);
		total_blocks += file->statInfo.st_blocks;
	}
	if (km_printf("total %llu\n", total_blocks) < 0) {
		set_status(LS_SERIOUS_ERROR);
	}
}

static void print_directory_name(const km_vector_file* directories, const ls_file* file)
{
	if (files_printed(0) > 0 || directories->size > 1)
	{
		const char* newLine = (files_printed(0) == 0) ? "" : "\n";
		if (km_printf("%s%s:\n", newLine, file->path) < 0) {
			return set_status(LS_SERIOUS_ERROR);
		}
		add_file_printed();
	}
}

static void list_directories(const km_vector_file* directories, ls_flags flags)
{
	for (size_t i = 0; get_status() != LS_SERIOUS_ERROR && i < directories->size; i++)
	{
		const ls_file* file = &(directories->arr[i]);

		print_directory_name(directories, file);	
		if (get_status() == LS_SERIOUS_ERROR) {
			return ;
		}	

		km_vector_file directoryEntries;
		km_vector_file_initialise(&directoryEntries, directories->destroy_element, directories->deep_copy);
		get_files_in_directory(file->path, flags, &directoryEntries);
		
		if (status_success())
		{
			sort(&directoryEntries, flags);

			if (flags.long_format) {
				list_total_blocks(&directoryEntries);
			}
		}

		if (status_success()) {
			print_files(&directoryEntries, flags);
		}

		if (status_success() && flags.recursive) {
			list_subdirectories(&directoryEntries, flags);
		}
		km_vector_file_destroy(&directoryEntries);
	}
}

void list_operands(const km_vector_file* files, const km_vector_file* directories, ls_flags flags)
{
	if (files != NULL) {
		print_files(files, flags);
	}
	if (status_success() && directories != NULL) {
		list_directories(directories, flags);
	}
}
