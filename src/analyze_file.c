/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/09 17:37:24 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "directory.h"
#include "options.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <fcntl.h>

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
		ft_printf("%s:\n", file_name);
	}
	while ((entry = readdir(dir)))
	{
		if (*opt & OPT_ACAPS
			&& (ft_strequ(entry->d_name, ".") || ft_strequ(entry->d_name, "..")))
			continue;
		else if (!(*opt & OPT_A || *opt & OPT_ACAPS) && entry->d_name[0] == '.')
			continue;
		len += ft_strlen(entry->d_name) + 2;
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
	//	If not a tty (i.e. if piped), used the current term size..
	if (!isatty(STDOUT_FILENO))
	{

		int fd = open("/dev/tty", O_RDONLY);
		if (fd != -1)
		{
			ioctl(fd, TIOCGWINSZ, &winsize);
		}
		close(fd);
	}
	else
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	if (*opt & OPT_CCAPS && len > winsize.ws_col)
		print_dlist_col(dlst, len, winsize.ws_col, *opt);
	else
		print_dlist(dlst, *opt);
	if (*opt & OPT_RCAPS)
		analyze_list(dlst, *opt);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}
