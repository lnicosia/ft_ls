/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 15:04:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/26 12:15:01 by lnicosia         ###   ########.fr       */
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
	ft_printf("total %d\n", size / 1000);
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