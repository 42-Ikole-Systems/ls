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

#define LS_PARSE_ERROR -1

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
bool parse(int argc, const char** argv, ls_flags* flags);
