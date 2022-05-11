/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 15:18:50 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include "ls_padding.h"
#include <sys/ioctl.h>

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

int				(*get_compare_func(unsigned long long opt))(void*, void*)
{
	if ((opt & OPT_UCAPS && !(opt & OPT_T)) || !(opt & OPT_SORT))
		return (compare_none);
	if (opt & OPT_C)
	{
		if (!(opt & OPT_L || opt & OPT_G || opt & OPT_N
			|| opt & OPT_O) || opt & OPT_T)
			return (compare_ctimes);
	}
	if (opt & OPT_U)
	{
		if (!(opt & OPT_L || opt & OPT_G || opt & OPT_N
			|| opt & OPT_O) || opt & OPT_T)
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

int				print_files(t_file* files, size_t len, unsigned long long *opt)
{
	int				nb_files;
	t_ls_padding	padding;
	t_winsize		winsize;
	blksize_t		dir_size;
	t_file			file;

	if (!files)
		return (-1);
	nb_files = 0;
	ft_bzero(&padding, sizeof(padding));
	if (*opt & OPT_L || *opt & OPT_G || *opt & OPT_O)
		padding = get_padding(files, len, &dir_size, *opt);
	ft_bzero(&winsize, sizeof(winsize));
	if (isatty(STDOUT_FILENO) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize))
		return (ft_perror(""));
	*opt |= OPT_PATH;
	for (size_t i = 0; i < len; i++)
	{
		file = files[i];
		if (S_ISDIR(file.stats.st_mode) && !(*opt & OPT_D))
			continue;
		if (i != 0)
		{
			if (*opt & OPT_M)
				ft_printf(", ");
			else if (*opt & OPT_CCAPS)
				ft_printf("  ");
			else
				ft_printf("\n");
		}
		print_file(file.stats, file.name, padding, *opt);
		nb_files++;
	}
	if (nb_files > 0)
		ft_printf("\n");
	if (nb_files > 0 && *opt & OPT_MULTIPLE_DIRS)
		*opt |= OPT_NEWLINE;
	*opt &= ~OPT_ERROR;
	*opt &= ~OPT_PATH;
	return (0);
}

/*
**	Print the content of all the directories arguments
*/

int				print_directories(t_file* files, size_t len, unsigned long long *opt)
{
	if (!files)
		return (-1);
	for (size_t i = 0; i < len; i++)
	{
		if (!S_ISDIR(files[i].stats.st_mode))
			continue;
		analyze_directory(files[i].name, opt);
	}
	return (0);
}

t_dlist		*analyze_args(int ac, char **av, unsigned long long *opt)
{
	int			i;
	int			nb_dir;
	int			nb_files;
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
	nb_files = 0;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			i++;
			continue;
		}
		if (stat(av[i], &file.stats))
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
		else
		{
			if (S_ISLNK(file.stats.st_mode))
			{
				ft_printf("Link\n");
			}
			nb_files++;
		}
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
		if (*opt & OPT_R && *opt & OPT_SORT)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
		i++;
	}
	if (nb_dir > 1 || (nb_dir > 0 && nb_files > 0))
	{
		*opt |= OPT_MULTIPLE_DIRS;
	}
	return (dlst);
}

int				ft_ls(int ac, char **av)
{
	unsigned long long	opt;
	int		real_args;
	t_dlist	*dlst;
	t_file*	files;
	size_t	lstlen;

	opt = OPT_SORT;
	dlst = NULL;
	real_args = ac - 1;
	if (parse_ls_options(ac, av, &opt, &real_args))
		return (-1);
	if (real_args == 0)
	{
		analyze_directory(".", &opt);
	}
	else
	{
		dlst = analyze_args(ac, av, &opt);
	}
	if (!(files = (t_file*)ft_dlist_to_array(dlst)))
		return (-1);
	lstlen = ft_dlstlen(dlst);
	print_files(files, lstlen, &opt);
	if (!(opt & OPT_D))
		print_directories(files, lstlen, &opt);
	ft_memdel((void**)&files);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}
