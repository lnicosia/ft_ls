/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 15:57:56 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "directory.h"
#include <sys/types.h>
#include <dirent.h>
#include "options.h"

int		analyze_directory(char *name, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;

	(void)opt;
	ft_printf("Directory\n");
	if (!(dir = opendir(name)))
	{
		return (ft_perror(""));
	}
	while ((entry = readdir(dir)))
	{
		if (opt & OPT_A || entry->d_name[0] != '.')
			ft_printf("%s ", entry->d_name);
	}
	if (closedir(dir))
	{
		return (ft_perror(""));
	}
	return (0);
}

int		analyze_file(char *file, int opt)
{
	t_stat	file_stats;

	ft_printf("Analyzing %s\n", file);
	if (lstat(file, &file_stats))
	{
		ft_printf("ft_ls: cannot access '%s': ", file);
		return (ft_perror(""));
	}
	if (S_ISDIR(file_stats.st_mode))
		analyze_directory(file, opt);
	else if (S_ISLNK(file_stats.st_mode))
		ft_printf("Link\n");
	else if (S_ISCHR(file_stats.st_mode))
		ft_printf("Character periph\n");	
	else if (S_ISBLK(file_stats.st_mode))
		ft_printf("Block\n");
	else if (S_ISFIFO(file_stats.st_mode))
		ft_printf("Fifo\n");
	else if (S_ISSOCK(file_stats.st_mode))
		ft_printf("Socket\n");
	else
		ft_printf("File\n");
	return (0);
}
