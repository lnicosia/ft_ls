/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 14:24:43 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
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
**	Assumes it's made of char*
*/

void	print_dlist(t_dlist *dlst)
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
		ft_printf("%s", (char*)dlst->content);
		dlst = dlst->next;
	}
	ft_printf("\n");
}

/*
**	Print all the non-directory file names
*/

int		print_files(int ac, char **av)
{
	int		i;
	t_stat	stats;
	t_dlist	*new;
	t_dlist	*dlst;

	dlst = NULL;
	i = 1;
	while (i < ac)
	{
		if (lstat(av[i], &stats))
			return (ft_perror("Error :"));
		if (S_ISDIR(stats.st_mode))
		{
			i++;
			continue;
		}
		if (!(new = ft_dlstnew(av[i], ft_strlen(av[i]) + 1)))
		{
			ft_dlstdelfront(&dlst);
			return (ft_perror("Error: "));
		}
		ft_dlstinsert(&dlst, new, compare);
		i++;
	}
	print_dlist(dlst);
	ft_dlstdelfront(&dlst);
	return (0);
}

int		ft_ls(int ac, char **av)
{
	int	i;
	int	opt;
	int	real_args;

	opt = 0;
	real_args = ac - 1;
	parse_ls_options(ac, av, &opt, &real_args);
	print_files(ac, av);
	i = 1;
	while (i < ac)
	{
		if (!is_arg_an_option_line(av[i]))
			analyze_args(av[i], opt);
		i++;
	}
	if (real_args == 0)
	{
		analyze_args(".", opt);
	}
	return (0);
}