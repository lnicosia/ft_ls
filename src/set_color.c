/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 18:18:46 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/18 19:01:06 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "options.h"
#include "ls_colors.h"

void	init_env_colors(char* env_colors[100])
{
	char*	env_str;
	char**	colors;
	size_t	i;

	env_str = getenv("LS_COLORS");
	if (env_str == NULL)
		return ;
	//ft_printf("Colors = \n%s\n", ls_colors);
	colors = ft_strsplit(env_str, ':');
	if (colors == NULL)
		return ;
	i = 0;
	while (colors[i])
	{
		ft_printf("%s\n", colors[i]);
		if (i < 100)
			env_colors[i] = 0;
		i++;
	}
}

/*
**	Sets the right color according to the type of a file
*/

void	set_color(char *file, mode_t mode, unsigned long long opt)
{
	static char		env_colors[100][100];
	static int		first_call = 1;

	if (first_call == 1)
	{
		init_env_colors(env_colors);
		first_call = 0;
	}
	if (opt & OPT_B && !S_ISREG(mode))
		ft_printf("{bold}");
	if (S_ISDIR(mode))
	{
		if ((mode & S_IWUSR) && (mode & S_IWGRP) && (mode & S_IWOTH))
			ft_printf("{reset}{blue}{bgreen}");
		else
			ft_printf("{blue}");
	}
	else if (S_ISLNK(mode))
	{
		if (is_link_valid(file))
			ft_printf("{cyan}");
		else
			ft_printf("{red}");
	}
	else if (S_ISCHR(mode))
		ft_printf("{bblack}{yellow}");
	else if (S_ISBLK(mode))
		ft_printf("{reset}");
	else if (S_ISFIFO(mode))
		ft_printf("{reset}{bblack}{yellow}");
	else if (S_ISSOCK(mode))
		ft_printf("{reset}");
	else if (S_ISREG(mode))
	{
		if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH))
		{
			if (opt & OPT_B)
				ft_printf("{bold}");
			ft_printf("{green}");
		}
	}
}
