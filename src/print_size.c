/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 15:04:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 21:14:24 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include <sys/stat.h>

/*
**	Print file size shortened
*/

void	print_size_short(off_t size)
{
	char	letters[6];
	int		type;
	float	floaty;

	letters[0] = ' ';
	letters[1] = 'K';
	letters[2] = 'M';
	letters[3] = 'G';
	letters[4] = 'T';
	floaty = (float)size;
	type = 0;
	while (floaty > 10)
	{
		floaty /= 1000;
		type++;
	}
	ft_printf("%.1f%c ", floaty, letters[type]);
}

/*
**	Print file size
*/

void	print_size(off_t size, int padding, int opt)
{
	if (opt & OPT_L)
		ft_printf("%*ld ", padding, size);
	else
		ft_printf("%ld ", size);
}