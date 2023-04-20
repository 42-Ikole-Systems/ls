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

	typedef struct s_ls_flags
	{
		int hidden_directories			: 1; // [a] include hidden directories
		int long_format					: 1; // [l] long format
		int recursive					: 1; // [R] Recusively list subdirectories
		int reverse_lexi_sort			: 1; // [r] reverse lexicographical sort
		int time_sort					: 1; // [t] sort by time modified
		int use_access_time				: 1; // [u] Use time of last access, instead of last modification of the file for sorting and in long format.
		int no_sort						: 1; // [f] Output is not sorted.  This option turns on the -a option.
		int display_groupname			: 1; // [g] display the group name in the long (-l) format output (the owner name is suppressed and long format is turned on).
		int display_directory_as_file	: 1; // [d] Directories are listed as plain files (not searched recursively).
		int colorised_output			: 1; // [G] Gives the output Beautiful colors :D
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
