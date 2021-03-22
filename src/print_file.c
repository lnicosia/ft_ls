/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 16:49:26 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 18:08:02 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include <sys/stat.h>

/*
**	Print the type of the file as the first letter [-dbclps]
*/

void	print_type(mode_t mode)
{
	if (S_ISDIR(mode))
		ft_printf("d");
	else if (S_ISLNK(mode))
		ft_printf("l");
	else if (S_ISCHR(mode))
		ft_printf("c");	
	else if (S_ISBLK(mode))
		ft_printf("b");
	else if (S_ISFIFO(mode))
		ft_printf("p");
	else if (S_ISSOCK(mode))
		ft_printf("s");
	else
		ft_printf("-");
}

/*
**	Print permissions of a file under the 'drwxrwxrwx' format
*/

void	print_permissions(mode_t mode)
{
	(void)mode;
	ft_printf("xxxxxxxxx ");
}

/*
**	Print the file name with colors.
**	Cuts the path after the last '/'
*/

void	print_file_name(t_stat file_stats, char *file)
{
	char	*name;

	if (!(name = ft_strrchr(file, '/')))
		name = file;
	else
		name++;
	if (S_ISDIR(file_stats.st_mode))
		ft_printf("{blue}");
	else if (S_ISLNK(file_stats.st_mode))
		ft_printf("{cyan}");
	else if (S_ISCHR(file_stats.st_mode))
		ft_printf("{yellow}");	
	else if (S_ISBLK(file_stats.st_mode))
		ft_printf("{reset}");
	else if (S_ISFIFO(file_stats.st_mode))
		ft_printf("{reset}");
	else if (S_ISSOCK(file_stats.st_mode))
		ft_printf("{reset}");
	else
		ft_printf("{green}");
	ft_printf("%s{reset}", name);
}

/*
**	Print the detailed '-l' format of a file
*/

void	print_details(t_stat file_stats, char *file, int opt)
{
	(void)opt;
	print_type(file_stats.st_mode);
	print_permissions(file_stats.st_mode);
	print_file_name(file_stats, file);
	ft_printf("\n");
}

/*
**	Print a file according to the options
*/

void	print_file(t_stat file_stats, char *file, int opt)
{
	if (opt & OPT_L)
		print_details(file_stats, file, opt);
	else
		print_file_name(file_stats, file);
}