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

static bool get_flag(ls_flags* flags, char c)
{
	switch (c)
	{
		case 'a': flags->hidden_directories = true; break;
		case 'l': flags->long_format = true; break;
		case 'R': flags->recursive = true; break;
		case 'r': flags->reverse_lexi_sort = true; break;
		case 't': flags->time_sort = true; break;
		case 'u': flags->use_access_time = true; break;
		case 'f': flags->no_sort = true; break;
		case 'g': flags->display_groupname = true; break;
		case 'd': flags->display_directory_as_file = true; break;
		case 'G': flags->colorised_output = true; break;
		default:
			return false;
	}
	return true;
}

ls_status parse_flags(const char* token, ls_flags* flags)
{
	if (token[0] != '-') {
		km_printf("Trying to parse flag, but token does not start with a `-` [%s]\n", token);
		return LS_SERIOUS_ERROR;
	}

	for (size_t i = 1; token[i]; ++i)
	{
		
		if (get_flag(flags, token[i]) == false)
		{
			km_printf("ls: illegal option -- %c\n", token[i]);
			return LS_SERIOUS_ERROR;
		}
	}
	return LS_SUCCESS;
}
