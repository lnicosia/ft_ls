/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 11:06:42 by lnicosia          #+#    #+#             */
/*   Updated: 2021/07/06 13:53:48 by lnicosia         ###   ########.fr       */
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
		*opt &= ~OPT_CCAPS;
		*opt |= OPT_L;
		*opt |= OPT_TOTAL;
	}
		else if (av == 'u')
	{
		*opt |= OPT_U;
	}
	else if (av == 'f')
	{
		*opt &= ~OPT_L;
		*opt |= OPT_F;
		*opt |= OPT_UCAPS;
		*opt |= OPT_A;
	}
	else if (av == 'g')
	{
		*opt |= OPT_G;
	}
	else if (av == 'G')
	{
		*opt |= OPT_GCAPS;
	}
	else if (av == 'd')
	{
		*opt |= OPT_D;
	}
	else if (av == 'M')
	{
		*opt |= OPT_MCAPS;
	}
	else if (av == '0')
	{
		*opt |= OPT_0;
	}
	else if (av == 'c')
	{
		*opt |= OPT_C;
	}
	else if (av == 'C')
	{
		*opt &= ~OPT_L;
		*opt |= OPT_CCAPS;
	}
	else if (av == 'U')
	{
		*opt |= OPT_UCAPS;
	}
	else if (av == 'T')
	{
		*opt |= OPT_TCAPS;
	}
	else if (av == 'b')
	{
		*opt |= OPT_B;
	}
	else if (av == '1')
	{
		*opt |= OPT_1;
	}
	return (0);
}

int		parse_option_line(char *av, int *opt)
{
	av++;
	while (*av)
	{
		check_opt(*av, opt);
		av++;
	}
	return (0);
}
