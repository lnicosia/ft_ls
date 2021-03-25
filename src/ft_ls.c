/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 18:46:32 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include <sys/stat.h>

/*
**	Checks if the given string is an option line (starting with '-')
*/

int		is_arg_an_option_line(char *av)
{
	return (ft_strlen(av) >= 1 && av[0] == '-');
}

/*
**	Parse all the options by checking arguments starting with '-'
*/

int		parse_ls_options(int ac, char **av, int *opt, int *real_args)
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
**	Print the list content
**	Assumes it's made of t_file*
*/

void	print_dlist(t_dlist *dlst, int opt)
{
	int	first;

	if (!dlst)
		return ;
	first = 1;
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	while (dlst)
	{
		if (first)
		{
			first = 0;
		}
		else
			ft_printf("  ");
		print_file(((t_file*)dlst->content)->stats,
		((t_file*)dlst->content)->name, opt);
		dlst = dlst->next;
	}
	ft_printf("\n");
}

/*
**	Frees the content of a t_file
*/

void	free_t_file(void *file, size_t size)
{
	(void)size;
	free(((t_file*)file)->name);
}

/*
**	Print all the non-directory file names
*/

int		print_files(int ac, char **av, int *new_line, int opt)
{
	int		i;
	t_file	file;
	t_dlist	*new;
	t_dlist	*dlst;
	int		(*compare_func)(void *, void *);

	dlst = NULL;
	if (opt & OPT_T)
		compare_func = compare_times;
	else
		compare_func = compare_names;
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
		if (!(file.name = ft_strdup(av[i])))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			return (ft_perror(""));
		}
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
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
	print_dlist(dlst, opt);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}

int		ft_ls(int ac, char **av)
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