/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 15:16:38 by lnicosia         ###   ########.fr       */
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
**
*/

void	analyze_list(t_dlist *lst, int opt)
{
	t_file	*file;
	
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		file = (t_file*)lst->content;
		if (!S_ISDIR(file->stats.st_mode))
		{
			lst = lst->next;
			continue;
		}
		analyze_directory(file->name, opt);
		lst = lst->next;
	}
}

/*
**	Analyse the current directory
**	prints all its files
**	then if '-R' was given, explore it
*/

int		analyze_directory(char *file_name, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;
	t_file			file;
	t_dlist			*dlst;
	t_dlist			*new;

	if (opt & OPT_RCAPS)
		ft_printf("\n\n%s:\n", file_name);
	dlst = NULL;
	if (!(dir = opendir(file_name)))
	{
		return (ft_perror("opendir error"));
	}
	while ((entry = readdir(dir)))
	{
		if (!(opt & OPT_A) && entry->d_name[0] == '.')
			continue;
		if (!(path = ft_strjoin(file_name, "/")))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror(""));
		}
		if (!(path = ft_strjoin_free(path, entry->d_name)))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		if (lstat(path, &file.stats))
		{
			ft_printf("ft_ls: cannot access '%s': ", path);
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		file.name = path;
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		ft_dlstinsert(&dlst, new, compare);
	}
	if (closedir(dir))
	{
		ft_dlstdelfront(&dlst, free_t_file);
		return (ft_perror(""));
	}
	print_dlist(dlst, opt);
	if (opt & OPT_RCAPS)
		analyze_list(dlst, opt);
	ft_dlstdelfront(&dlst, free_t_file);
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