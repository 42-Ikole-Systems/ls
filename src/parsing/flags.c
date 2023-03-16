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
		case 'a': return a;
		case 'l': return l;
		case 'R': return R;
		case 'r': return r;
		case 't': return t;
		case 'u': return u;
		case 'f': return f;
		case 'g': return g;
		case 'd': return d;
		default:
			return unknown;
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
		if (tmp == unknown)
		{
			km_printf("ls: illegal option -- %c\n", token[i]);
			return LS_PARSE_ERROR;
		}
		*flags &= tmp;
	}
	return flags_found;
}
