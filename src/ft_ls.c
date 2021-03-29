/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 17:32:12 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include "ls_padding.h"
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
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

int				get_snblen(long int nb)
{
	int	len;

	len = 0;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int				get_nblen(long unsigned int nb)
{
	int	len;

	len = 0;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int				get_userlen(t_file *file)
{
	t_passwd	*passwd;

	if (!(passwd = getpwuid(file->stats.st_uid)))
	{
		ft_perror("Error: ");
		return (0);
	}
	return ((int)ft_strlen(passwd->pw_name));
}

int				get_grouplen(t_file *file)
{
	t_group	*group;

	if (!(group = getgrgid(file->stats.st_gid)))
	{
		ft_perror("Error: ");
		return (0);
	}
	return ((int)ft_strlen(group->gr_name));
}

long int		get_block_size(t_file *file)
{
	long int	size;

	size = 0;
	size = file->stats.st_blocks * 512;
	return (size);
}

t_ls_padding	get_padding(t_dlist *dlst, blksize_t *dir_size)
{
	t_ls_padding		padding;
	int					len;
	long int			size;
	long unsigned int	usize;

	ft_bzero(&padding, sizeof(padding));
	while (dlst)
	{
		size = get_block_size(((t_file*)(dlst->content)));
		(*dir_size) += size;
		len = get_snblen(((t_file*)(dlst->content))->stats.st_size);
		if (len > padding.size)
			padding.size = len;
		usize = ((t_file*)(dlst->content))->stats.st_nlink;
		len = get_nblen(usize);
		if (len > padding.links)
			padding.links = len;
		len = get_userlen((t_file*)dlst->content);
		if (len > padding.user)
			padding.user = len;
		len = get_grouplen((t_file*)dlst->content);
		if (len > padding.group)
			padding.group = len;
		dlst = dlst->next;
	}
	return (padding);
}

/*
**	Print the list content
**	Assumes it's made of t_file*
*/

void			print_dlist(t_dlist *dlst, int opt)
{
	int				first;
	t_ls_padding	padding;
	blksize_t		dir_size;

	if (!dlst)
		return ;
	first = 1;
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	dir_size = 0;
	if (opt & OPT_L)
	{
		padding = get_padding(dlst, &dir_size);
		ft_printf("total %ld\n", dir_size / 1024);
	}
	while (dlst)
	{
		if (first)
		{
			first = 0;
		}
		else if (!(opt & OPT_L) && isatty(STDOUT_FILENO))
			ft_printf("  ");
		print_file(((t_file*)dlst->content)->stats,
		((t_file*)dlst->content)->name, padding, opt);
		dlst = dlst->next;
	}
	if (!(opt & OPT_L) && isatty(STDOUT_FILENO))
		ft_printf("\n");
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
**
*/

int				(*get_compare_func(int opt))(void*, void*)
{
	if (opt & OPT_F || opt & OPT_UCAPS)
		return (compare_none);
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

int				print_files(int ac, char **av, int *new_line, int opt)
{
	int			i;
	t_file		file;
	t_dlist		*new;
	t_dlist		*dlst;
	size_t		len;
	int			(*compare_func)(void *, void *);
	t_winsize	winsize;

	dlst = NULL;
	compare_func = get_compare_func(opt);
	len = 0;
	i = 1;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			i++;
			continue;
		}
		if (lstat(av[i], &file.stats))
		{
			ft_printf("ft_ls: cannot access '%s': ", av[i]);
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror(""));
		}
		if (S_ISDIR(file.stats.st_mode))
		{
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
		if (opt & OPT_R)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
		(*new_line)++;
		i++;
	}
	ft_bzero(&winsize, sizeof(winsize));
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	if (opt & OPT_CCAPS && len > winsize.ws_col && isatty(STDOUT_FILENO))
		print_dlist_col(dlst, len, winsize.ws_col, opt);
	else
		print_dlist(dlst, opt);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}

int				ft_ls(int ac, char **av)
{
	int	i;
	int	new_line;
	int	opt;
	int	real_args;

	opt = 0;
	new_line = 0;
	real_args = ac - 1;
	parse_ls_options(ac, av, &opt, &real_args);
	print_files(ac, av, &new_line, opt);
	i = 1;
	while (i < ac)
	{
		if (!is_arg_an_option_line(av[i]))
			analyze_args(av[i], new_line, opt);
		i++;
	}
	if (real_args == 0)
	{
		analyze_args(".", new_line, opt);
	}
	return (0);
}