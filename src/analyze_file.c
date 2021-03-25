/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 14:16:39 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "directory.h"
#include "options.h"
#include <sys/types.h>
#include <dirent.h>

/*
**	Print a directory's content
*/

int		print_directory(char *file, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;
	t_stat			file_stats;	

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
		if (lstat(path, &file_stats))
		{
			ft_printf("ft_ls: cannot access '%s': ", path);
			return (ft_perror(""));
		}
		print_file(file_stats, entry->d_name, opt);
		ft_printf("  ");
		ft_strdel(&path);
	}
	if (closedir(dir))
	{
		return (ft_perror(""));
	}
	return (0);
}

/*
**	Analyse the current directory
**	prints all its files
**	then if '-R' was given, explore it
*/

int		analyze_directory(char *file, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;
	t_stat			file_stats;
	t_dlist			*dlst;
	t_dlist			*new;

	if (opt & OPT_RCAPS)
		ft_printf("\n\n%s:\n", file);
	//print_directory(file, opt);
	//if (!(opt & OPT_RCAPS))
	//	return (0);
	dlst = NULL;
	if (!(dir = opendir(file)))
	{
		return (ft_perror(""));
	}
	while ((entry = readdir(dir)))
	{
		if (!(opt & OPT_A) && entry->d_name[0] == '.')
			continue;
		if (!(path = ft_strjoin(file, "/")))
		{
			ft_dlstdelfront(&dlst);
			return (ft_perror(""));
		}
		if (!(path = ft_strjoin_free(path, entry->d_name)))
		{
			ft_dlstdelfront(&dlst);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		if (lstat(path, &file_stats))
		{
			ft_printf("ft_ls: cannot access '%s': ", path);
			ft_dlstdelfront(&dlst);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		if (!(new = ft_dlstnew(entry->d_name, ft_strlen(entry->d_name) + 1)))
		{
			ft_dlstdelfront(&dlst);
			ft_strdel(&path);
			return (ft_perror("Error: "));
		}
		//ft_printf("Adding %s to the list\n", new->content);
		ft_dlstinsert(&dlst, new, compare);
		if (!S_ISDIR(file_stats.st_mode))
		{
			ft_strdel(&path);
			continue ;
		}
		//analyze_directory(path, opt);
		ft_strdel(&path);
		//ft_printf("  ");
	}
	if (closedir(dir))
	{
		ft_dlstdelfront(&dlst);
		return (ft_perror(""));
	}
	print_dlist(dlst);
	ft_dlstdelfront(&dlst);
	return (0);
}

/*
**	Handles the arg differently than the rest
*/

int		analyze_args(char *file, int opt)
{
	t_stat		file_stats;

	if (lstat(file, &file_stats))
	{
		ft_printf("ft_ls: cannot access '%s': ", file);
		return (ft_perror(""));
	}
	if (S_ISDIR(file_stats.st_mode))
		analyze_directory(file, opt);
	return (0);
}