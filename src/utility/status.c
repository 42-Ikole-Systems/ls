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

static ls_status current_status = LS_SUCCESS;
static ls_status global_status = LS_SUCCESS;

ls_status get_status()
{
	return current_status;
}

ls_status get_global_status()
{
	return global_status;
}

bool status_success()
{
	return current_status == LS_SUCCESS;
}

void set_status(ls_status status)
{
	if (status > global_status) {
		global_status = status;
	}
	current_status = status;
}
