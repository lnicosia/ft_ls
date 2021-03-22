/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ls_options.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 11:06:42 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 14:50:12 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "libft.h"

int		check_opt(char av, int *opt)
{
	if (av == 'a')
	{
		*opt |= OPT_A;
	}
	else if (av == 'R')
	{
		*opt |= OPT_RCAPS;
	}
	else if (av == 'r')
	{
		*opt |= OPT_R;
	}
	else if (av == 't')
	{
		*opt |= OPT_T;
	}
	else if (av == 'l')
	{
		*opt |= OPT_L;
	}
	return (0);
}

int		parse_ls_options(char *av, int *opt)
{
	av++;
	while (*av)
	{
		check_opt(*av, opt);
		av++;
	}
	return (0);
}