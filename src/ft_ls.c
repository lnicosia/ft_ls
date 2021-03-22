/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 18:06:41 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"

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

int		ft_ls(int ac, char **av)
{
	int	i;
	int	opt;
	int	real_args;

	opt = 0;
	real_args = ac - 1;
	parse_ls_options(ac, av, &opt, &real_args);
	i = 1;
	while (i < ac)
	{
		if (!is_arg_an_option_line(av[i]))
		{
			analyze_file(av[i], opt);
		}
		i++;
	}
	if (real_args == 0)
	{
		analyze_file(".", opt);
	}
	ft_printf("\n");
	return (0);
}