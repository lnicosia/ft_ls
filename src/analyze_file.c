/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 19:17:41 by lnicosia         ###   ########.fr       */
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

void	analyze_list(t_dlist *lst, unsigned long long opt)
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
		analyze_directory(file->name, &opt);
		lst = lst->next;
	}
}

/*
**	Analyse the current directory
**	prints all its files
**	then if '-R' was given, explore it
*/

int		analyze_directory(char *file_name, unsigned long long *opt)
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
	int				special_chars;
	size_t			filename_len;

	dlst = NULL;
	len = 0;
	compare_func = get_compare_func(*opt);
	if (!(dir = opendir(file_name)))
	{
		custom_error("ft_ls: cannot open directory '%s': ", file_name);
		return (ft_perror(""));
	}
	if (*opt & OPT_RCAPS || *opt & OPT_NEWLINE || *opt & OPT_MULTIPLE_DIRS)
	{
		if (*opt & OPT_NEWLINE)
			ft_printf("\n");
		if (!(*opt & OPT_NEWLINE))
			*opt |= OPT_NEWLINE;
		special_chars = contains_special_chars(file_name);
		if (special_chars == 1)
		{
			if (*opt & OPT_NCAPS)
				ft_printf("%-s:\n", file_name);
			else
				ft_printf("'%-s':\n", file_name);
		}
		else if (special_chars == 2)
		{
			if (*opt & OPT_NCAPS)
				ft_printf("%-s:\n", file_name);
			else
				ft_printf("\"%-s\":\n", file_name);
		}
		else if (special_chars == 3)
		{
			if (*opt & OPT_NCAPS)
			{
				char* replaced = NULL;
				if (!(replaced = replace_char(file_name, '\n', "?")))
					ft_perror("replace_char:");
				ft_printf("%-s:\n", replaced);
				ft_strdel(&replaced);
			}
			else
			{
				char* replaced = NULL;
				if (!(replaced = replace_char(file_name, '\n', "'$'\\n''")))
					ft_perror("replace_char:");
				ft_printf("'%-s':\n", replaced);
				ft_strdel(&replaced);
			}
		}
		else
			ft_printf("%s:\n", file_name);
	}
	while ((entry = readdir(dir)))
	{
		if (*opt & OPT_ACAPS
			&& (ft_strequ(entry->d_name, ".") || ft_strequ(entry->d_name, "..")))
			continue;
		else if (!(*opt & OPT_A || *opt & OPT_ACAPS) && entry->d_name[0] == '.')
			continue;
		filename_len = ft_strlen(entry->d_name) + 2;
		if (isatty(STDOUT_FILENO))
		{
			special_chars = contains_special_chars(entry->d_name);
			if (special_chars == 1 || special_chars == 2)
			{
				if (!(*opt & OPT_NCAPS))
				{
					filename_len += 2;
					*opt |= OPT_SPECIAL_CHAR;
				}
			}
			else if (special_chars == 3)
			{
				if (!(*opt & OPT_NCAPS))
				{
					filename_len += 8;
					*opt |= OPT_SPECIAL_CHAR;
				}
				else
					filename_len += 1;
			}
		}
		len += filename_len;
		if (!(path = ft_strjoin(file_name, "/")))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror("ft_strjoin"));
		}
		if (!(path = ft_strjoin_free(path, entry->d_name)))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror("ft_strjoin_free"));
		}
		if (lstat(path, &file.stats))
		{
			custom_error("ft_ls: cannot access '%s': ", path);
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		if (*opt & OPT_P && S_ISDIR(file.stats.st_mode))
		{
			filename_len++;
			len++;
		}
		file.namelen = filename_len - 2;
		file.name = path;
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			return (ft_perror(""));
		}
		if (*opt & OPT_R && *opt & OPT_SORT)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
	}
	if (closedir(dir))
	{
		ft_dlstdelfront(&dlst, free_t_file);
		return (ft_perror(""));
	}
	if (!isatty(STDOUT_FILENO))
		winsize.ws_col = 80;
	else
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	t_file* files;
	if (!(files = (t_file*)ft_dlist_to_array(dlst)))
		return (-1);
	if (*opt & OPT_CCAPS && dlst)
		print_dlist_col(files, ft_dlstlen(dlst), len, winsize.ws_col, *opt);
	else
		print_dlist(files, ft_dlstlen(dlst), winsize.ws_col, *opt);
	if (*opt & OPT_RCAPS)
		analyze_list(dlst, *opt);
	ft_memdel((void**)&files);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}
