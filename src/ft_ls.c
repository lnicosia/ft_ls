/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/05 10:20:52 by lnicosia         ###   ########.fr       */
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

int				print_files(int ac, char **av, int *new_line, int *opt)
{
	int			i;
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
			(*opt) |= OPT_ERROR;
			i++;
			continue;
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
		if (*opt & OPT_R)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
		(*new_line)++;
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
	//ft_printf("Printing file args\n");
	parse_ls_options(ac, av, &opt, &real_args);
	print_files(ac, av, &new_line, &opt);
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