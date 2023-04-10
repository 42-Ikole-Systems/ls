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

static ls_flags get_flag(char c)
{
	switch (c)
	{
		case 'a': return flag_hidden_directories;
		case 'l': return flag_long_format;
		case 'R': return flag_recursive;
		case 'r': return flag_reverse_lexi_sort;
		case 't': return flag_time_sort;
		case 'u': return flag_use_access_time;
		case 'f': return flag_no_sort;
		case 'g': return flag_display_groupname;
		case 'd': return flag_display_directory_as_file;
		default:
			return flag_unknown;
	}
}

int parse_flags(const char* token, ls_flags* flags)
{
	if (token[0] != '-') {
		km_printf("Trying to parse flag, but token does not start with a `-` [%s]\n", token);
		return LS_PARSE_ERROR;
	}

	int		 flags_found = 0;
	ls_flags tmp;
	for (size_t i = 1; token[i]; ++i)
	{
		tmp = get_flag(token[i]);
		if (tmp == flag_unknown)
		{
			km_printf("ls: illegal option -- %c\n", token[i]);
			return LS_PARSE_ERROR;
		}
		*flags |= tmp;
	}
	return flags_found;
}
