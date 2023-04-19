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

#pragma once

#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>

///////////
// Types //
///////////

	typedef enum e_ls_status
	{
		LS_SUCCESS = 0,
		LS_MINOR_ERROR = 1,
		LS_SERIOUS_ERROR = 2
	} ls_status;

	typedef enum e_ls_flags
	{
		flag_unknown = 0x00,
		flag_hidden_directories			= 0x01,  // [a] 0000 0000 0000 0000 0000 0001 (include hidden directories)
		flag_long_format				= 0x02,  // [l] 0000 0000 0000 0000 0000 0010 (long format)
		flag_recursive					= 0x04,  // [R] 0000 0000 0000 0000 0000 0100 (Recusively list subdirectories)
		flag_reverse_lexi_sort			= 0x08,  // [r] 0000 0000 0000 0000 0000 1000 (reverse lexicographical sort)
		flag_time_sort					= 0x10,  // [t] 0000 0000 0000 0000 0001 0000 (sort by time modified)
		flag_use_access_time			= 0x20,  // [u] 0000 0000 0000 0000 0010 0000 (Use time of last access, instead of last modification of the file for sorting and in long format).
		flag_no_sort					= 0x41,  // [f] 0000 0000 0000 0000 0100 0001 (Output is not sorted.  This option turns on the -a option.)
		flag_display_groupname			= 0x82,  // [g] 0000 0000 0000 0000 1000 0010 (display the group name in the long (-l) format output (the owner name is suppressed).)
		flag_display_directory_as_file	= 0x100, // [d] 0000 0000 0000 0001 0000 0000 (Directories are listed as plain files (not searched recursively).)
		flag_colorised_output			= 0x200, // [G] 0000 0000 0000 0010 0000 0000 (Gives the output Beautiful colors :D)
	} ls_flags;

	#define ALLOWED_LS_FLAGS "alRrtufgdG"

	/*!
	 * @brief enum containing all filetypes
	*/
	typedef enum e_file_type
	{
		regular_file_type, // These are the most common type of file and contain data or program code.
		directory_type, // These are files that contain lists of other files and directories.
		symlink_type, // These are files that contain a reference to another file or directory.
		block_device_type, // These represent block-oriented devices such as hard disks, solid-state drives, and USB drives.
		character_device_type, // These represent character-oriented devices such as keyboards, mice, and serial ports.
		fifo_type, // (named pipes) These provide a mechanism for inter-process communication (IPC) on a single machine.
		unix_socket_type, // These are another type of IPC mechanism, similar to network sockets, but they are used for communication between processes on the same machine.
		UNKNOWN_TYPE
	} file_type;

	/*!
	 * @brief linke lijst with operands and their types
	*/
	typedef struct operand_list_s
	{
		const char*				path;
		const char*				filename;
		const char*				symlinkDestination;
		file_type				type;
		struct stat				statInfo;
		time_t					time;
		struct operand_list_s*	next;
	} operand_list_t;

	#define LS_CURRENT_DIRECTORY "."
