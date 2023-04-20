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
		case 'a': return (flags->hidden_directories = true);
		case 'l': return (flags->long_format = true);
		case 'R': return (flags->recursive = true);
		case 'r': return (flags->reverse_lexi_sort = true);
		case 't': return (flags->time_sort = true);
		case 'u': return (flags->use_access_time = true);
		case 'f': {
			flags->no_sort = true;
			flags->hidden_directories = true;
			return true;
		}
		case 'g': {
			flags->display_groupname = true;
			flags->long_format = true;
			return true;
		}
		case 'd': return (flags->display_directory_as_file = true);
		case 'G': return (flags->colorised_output = true);
		default:
			return false;
	}
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
