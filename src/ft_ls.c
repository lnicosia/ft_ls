/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:57 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 14:46:49 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"

int		ft_ls(int ac, char **av)
{
	int	i;
	int	opt;

	opt = 0;
	i = 1;
	while (i < ac)
	{
		if (ft_strlen(av[i]) >= 1 && av[i][0] == '-')
		{
			parse_ls_options(av[i], &opt);
		}
		i++;
	}
	i = 1;
	while (i < ac)
	{
		ft_printf("File = %s\n", av[i]);
		i++;
	}
	ft_printf("Options = %d\n", opt);
	return (0);
}