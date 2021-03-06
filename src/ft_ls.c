/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/05 15:13:23 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include "ls_padding.h"
#include <sys/ioctl.h>

/*
**	Checks if the given string is an option line (starting with '-')
*/

int				is_arg_an_option_line(char *av)
{
	return (ft_strlen(av) >= 1 && av[0] == '-');
}

/*
**	Parse all the options by checking arguments starting with '-'
*/

int				parse_ls_options(int ac, char **av, int *opt, int *real_args)
{
	int	i;
	
	i = 1;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			parse_option_line(av[i], opt);
			(*real_args)--;
		}
		i++;
	}
	return (0);
}

/*
**	Frees the content of a t_file
*/

void			free_t_file(void *file, size_t size)
{
	(void)size;
	free(((t_file*)file)->name);
}

/*
**	According to the options
**	get the right compare function that will be used
**	to sort our files
*/

int				(*get_compare_func(int opt))(void*, void*)
{
	if (opt & OPT_UCAPS && !(opt & OPT_T))
		return (compare_none);
	if (opt & OPT_C)
	{
		if (!(opt & OPT_L) || opt & OPT_T)
			return (compare_ctimes);
	}
	if (opt & OPT_U)
	{
		if (!(opt & OPT_L) || opt & OPT_T)
			return (compare_atimes);
	}
	if (opt & OPT_T)
	{
		if (opt & OPT_C)
			return (compare_ctimes);
		if (opt & OPT_U)
			return (compare_atimes);
		return (compare_times);
	}
	if (opt & OPT_MCAPS)
		return (compare_names_no_case);
	return (compare_names);
}

/*
**	Print all the non-directory file names
*/

int				print_files(t_dlist *dlst, int *opt)
{
	/*int			i;
	int			nb_dir;
	t_file		file;
	t_dlist		*new;
	t_dlist		*dlst;
	size_t		len;
	int			(*compare_func)(void *, void *);
	t_winsize	winsize;

	dlst = NULL;
	compare_func = get_compare_func(*opt);
	len = 0;
	i = 1;
	nb_dir = 0;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			i++;
			continue;
		}
		if (lstat(av[i], &file.stats))
		{
			custom_error("ft_ls: cannot access '%s': ", av[i]);
			ft_perror("");
			if (ft_strchr(av[i], ' '))
				(*opt) |= OPT_ERROR;
			i++;
			continue;
		}
		if (S_ISDIR(file.stats.st_mode))
		{
			nb_dir++;
			i++;
			continue;
		}
		len += ft_strlen(av[i]) + 2;
		if (!(file.name = ft_strdup(av[i])))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror(""));
		}
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
			ft_strdel(&file.name);
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror(""));
		}
		if (*opt & OPT_R)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
		*opt |= OPT_NEWLINE;
		i++;
	}
	ft_bzero(&winsize, sizeof(winsize));
	if (isatty(STDOUT_FILENO) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize))
		return (ft_perror(""));
	*opt |= OPT_PATH;
	if (*opt & OPT_CCAPS && len > winsize.ws_col && isatty(STDOUT_FILENO))
		print_dlist_col(dlst, len, winsize.ws_col, *opt);
	else
		print_dlist(dlst, *opt);
	if (*opt & OPT_L)
		*opt |= OPT_TOTAL;
	*opt &= ~OPT_ERROR;
	*opt &= ~OPT_PATH;
	ft_dlstdelfront(&dlst, free_t_file);
	*/
	int				first;
	int				nb_files;
	t_ls_padding	padding;
	t_file			*file;
	t_winsize		winsize;

	if (!dlst)
		return (-1);
	first = 1;
	nb_files = 0;
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	ft_bzero(&padding, sizeof(padding));
	ft_bzero(&winsize, sizeof(winsize));
	if (isatty(STDOUT_FILENO) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize))
		return (ft_perror(""));
	*opt |= OPT_PATH;
	while (dlst)
	{
		file = (t_file*)dlst->content;
		if (S_ISDIR(file->stats.st_mode))
		{
			dlst = dlst->next;
			continue;
		}
		if (first)
			first = 0;
		else if (!(*opt & OPT_L) && isatty(STDOUT_FILENO))
			ft_printf("  ");
		print_file(file->stats, file->name, padding, *opt);
		nb_files++;
		dlst = dlst->next;
	}
	if (nb_files > 0 && !(*opt & OPT_L) && isatty(STDOUT_FILENO))
		ft_printf("\n");
	if (*opt & OPT_L)
		*opt |= OPT_TOTAL;
	*opt &= ~OPT_ERROR;
	*opt &= ~OPT_PATH;
	return (0);
}

/*
**	Print the content of all the directories arguments
*/

int				print_directories(t_dlist *dlst, int *opt)
{
	t_file			*file;

	if (!dlst)
		return (-1);
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	while (dlst)
	{
		file = (t_file*)dlst->content;
		if (!S_ISDIR(file->stats.st_mode))
		{
			dlst = dlst->next;
			continue;
		}
		analyze_directory(file->name, opt);
		dlst = dlst->next;
	}
	return (0);
}

t_dlist		*analyze_args(int ac, char **av, int *opt)
{
	int			i;
	int			nb_dir;
	t_file		file;
	t_dlist		*new;
	t_dlist		*dlst;
	size_t		len;
	int			(*compare_func)(void *, void *);

	dlst = NULL;
	compare_func = get_compare_func(*opt);
	len = 0;
	i = 1;
	nb_dir = 0;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			i++;
			continue;
		}
		if (lstat(av[i], &file.stats))
		{
			custom_error("ft_ls: cannot access '%s': ", av[i]);
			ft_perror("");
			if (ft_strchr(av[i], ' '))
				(*opt) |= OPT_ERROR;
			i++;
			continue;
		}
		if (S_ISDIR(file.stats.st_mode))
			nb_dir++;
		len += ft_strlen(av[i]) + 2;
		if (!(file.name = ft_strdup(av[i])))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_perror("strdup");
			return (NULL);
		}
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
			ft_strdel(&file.name);
			ft_dlstdelfront(&dlst, free_t_file);
			ft_perror("lstnew");
			return (NULL);
		}
		if (*opt & OPT_R)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
		i++;
	}
	if (nb_dir > 1)
		*opt |= OPT_MULTIPLE_DIRS;
	return (dlst);
}

int				ft_ls(int ac, char **av)
{
	int		opt;
	int		real_args;
	t_dlist	*dlst;

	opt = 0;
	dlst = NULL;
	real_args = ac - 1;
	parse_ls_options(ac, av, &opt, &real_args);
	if (real_args == 0)
	{
		analyze_directory(".", &opt);
	}
	else
	{
		dlst = analyze_args(ac, av, &opt);
	}
	print_files(dlst, &opt);
	print_directories(dlst, &opt);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}