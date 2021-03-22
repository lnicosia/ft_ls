/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 18:08:00 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "directory.h"
#include "options.h"
#include <sys/types.h>
#include <dirent.h>

/*
**	Analyse the current directory
**	If the directory was an arg or if '-R' was given, explore it
**	otherwise, just print its name
*/

int		analyze_directory(int count, t_stat file_stats, char *file, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;

	if (count > 1 && !(opt & OPT_RCAPS))
	{
		print_file(file_stats, file, opt);
		return (0);
	}
	if (count > 1)
		ft_printf("\n\n%s:\n", file);
	if (!(dir = opendir(file)))
	{
		return (ft_perror(""));
	}
	while ((entry = readdir(dir)))
	{
		if (!(opt & OPT_A) && entry->d_name[0] == '.')
			continue;
		if (!(path = ft_strjoin(file, "/")))
			return (ft_perror(""));
		if (!(path = ft_strjoin_free(path, entry->d_name)))
			return (ft_perror(""));
		if (opt & OPT_A || entry->d_name[0] != '.')
			analyze_file(path, opt);
		ft_strdel(&path);
		ft_printf("\t");
	}
	if (closedir(dir))
	{
		return (ft_perror(""));
	}
	return (0);
}

int		analyze_file(char *file, int opt)
{
	t_stat		file_stats;
	static int	count = 0;

	count++;
	if (lstat(file, &file_stats))
	{
		ft_printf("ft_ls: cannot access '%s': ", file);
		return (ft_perror(""));
	}
	if (S_ISDIR(file_stats.st_mode))
		analyze_directory(count, file_stats, file, opt);
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
		print_file(file_stats, file, opt);
	return (0);
}
