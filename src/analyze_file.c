/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/31 14:47:48 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "directory.h"
#include "options.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>

/*
**	Analyze the directories contained in our sorted list
*/

void	analyze_list(t_dlist *lst, int new_line, int opt)
{
	t_file	*file;
	
	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		file = (t_file*)lst->content;
		if (!S_ISDIR(file->stats.st_mode)
		|| file->name[ft_strlen(file->name) - 1] == '.')
		{
			lst = lst->next;
			continue;
		}
		analyze_directory(file->name, new_line, opt);
		lst = lst->next;
	}
}

/*
**	Analyse the current directory
**	prints all its files
**	then if '-R' was given, explore it
*/

int		analyze_directory(char *file_name, int new_line, int opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;
	t_file			file;
	t_dlist			*dlst;
	t_dlist			*new;
	size_t			len;
	int				(*compare_func)(void *, void *);
	t_winsize		winsize;

	if (opt & OPT_RCAPS || new_line)
	{
		if (new_line)
			ft_printf("\n");
		if (!new_line)
			new_line++;
		ft_printf("%s:\n", file_name);
	}
	dlst = NULL;
	len = 0;
	compare_func = get_compare_func(opt);
	if (!(dir = opendir(file_name)))
	{
		return (ft_perror("opendir error"));
	}
	while ((entry = readdir(dir)))
	{
		if (!(opt & OPT_A) && entry->d_name[0] == '.')
			continue;
		len += ft_strlen(entry->d_name) + 2;
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
		if (opt & OPT_R)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
	}
	if (closedir(dir))
	{
		ft_dlstdelfront(&dlst, free_t_file);
		return (ft_perror(""));
	}
	ft_bzero(&winsize, sizeof(winsize));
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	if (opt & OPT_CCAPS && len > winsize.ws_col && isatty(STDOUT_FILENO))
		print_dlist_col(dlst, len, winsize.ws_col, opt);
	else
		print_dlist(dlst, opt);
	if (opt & OPT_RCAPS)
		analyze_list(dlst, new_line, opt);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}

/*
**	Handles the arg differently than the rest
*/

int		analyze_args(char *file, int new_line, int opt)
{
	t_stat		file_stats;

	if (lstat(file, &file_stats))
	{
		ft_printf("ft_ls: cannot access '%s': ", file);
		return (ft_perror(""));
	}
	if (S_ISDIR(file_stats.st_mode))
	{
		analyze_directory(file, new_line, opt);
	}
	return (0);
}