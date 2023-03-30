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

#include "stdbool.h"

///////////
// Types //
///////////

	typedef enum e_ls_flags
	{
		unknown = 0x00,
		a = 0x01,  // 0000 0000 0000 0000 0000 0001 (include hidden directories)
		l = 0x02,  // 0000 0000 0000 0000 0000 0010 (long format)
		R = 0x04,  // 0000 0000 0000 0000 0000 0100 (Recusively list subdirectories)
		r = 0x08,  // 0000 0000 0000 0000 0000 1000 (reverse lexicographical sort)
		t = 0x10,  // 0000 0000 0000 0000 0001 0000 (sort by time)
		u = 0x20,  // 0000 0000 0000 0000 0010 0000 (Use time of last access, instead of last modification of the file for sorting).
		f = 0x41,  // 0000 0000 0000 0000 0100 0001 (Output is not sorted.  This option turns on the -a option.)
		g = 0x80,  // 0000 0000 0000 0000 1000 0000 (display the group name in the long (-l) format output (the owner name is suppressed).)
		d = 0x100, // 0000 0000 0000 0001 0000 0000 (Directories are listed as plain files (not searched recursively).)
	} ls_flags;

	#define ALLOWED_LS_FLAGS "alRrtufgd"

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
		const char*				name;
		file_type				type;
		struct operand_list_s*	next;
	} operand_list_t;

	#define LS_PARSE_ERROR -1
	#define LS_ERROR -2

	#define LS_CURRENT_DIRECTORY "."

/////////////
// Parsing //
/////////////

	/*!
	* @brief parses command line arguments
	*
	* @param argc amount of arguments
	* @param argv the arguments
	* @param flags it will store the flags found in this variable
	*
	* @return true on success, false on error
	* 
	* @note it will ignore the first argument since this should be the name of the executable.
	*/
	bool parse(int argc, const char** argv, ls_flags* flags, operand_list_t** operand_list);

/////////////
// Utility //
/////////////

	/*!
	* @brief deallocates and cleans up the list
	*
	* @param list -
	*/
	void clear_list(operand_list_t* list);

	/*!
	* @brief creates and appends a new node to the end of the list
	*
	* @param list pointer to the list
	* @param name -
	* 
	* @return newly apended node, NULL if fails
	*/
	operand_list_t* list_append(operand_list_t** list, const char* name);


	/*!
	 * @brief appends node to end of list
	 * @param list pointer to the list
	 * @param newNode node to append
	 * @return newNode
	*/
	operand_list_t* list_append_node(operand_list_t** list, operand_list_t* newNode);
