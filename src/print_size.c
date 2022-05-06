/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 15:04:47 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/26 18:32:44 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include <sys/stat.h>

int				get_doublelen(double size)
{
	int	len;

	len = 0;
	long unsigned int nb = (long unsigned int)(size + 0.1);
	//ft_printf("nb = %f\n", size);
	//ft_printf("rounded nb = %ld\n", nb);
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	//ft_printf("len = %d\n", nb);
	return (len);
}

/*
**	Print file size shortened
*/

void	print_size_readable(off_t long_size, int padding, double divider)
{
	//	Padding - 1 because we add a letter to the output
	//padding--;
	char letter = ' ';
	double final_size = 0.0;
	double size = (double)long_size;
	if (size < divider)
	{
		ft_printf("%*ld ", padding + 1, (long int)size);
		return ;
	}
	else if (ft_ceil(size / divider) < divider)
	{
		final_size = size / divider;
		letter = 'K';
	}
	else if (ft_ceil(size / (divider * divider)) < divider)
	{
		final_size = size / (divider * divider);
		letter = 'M';
	}
	else if (ft_ceil(size / (divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider);
		letter = 'G';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider);
		letter = 'T';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider * divider);
		letter = 'P';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider * divider * divider))
		 < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider * divider * divider);
		letter = 'E';
	}
	else
	{
		final_size = size / (divider * divider * divider
			 * divider * divider * divider * divider);
		letter = '?';
	}
	int len = get_doublelen(final_size);
	if (len == 1 && letter != ' ')
	{
		//	Round up if not multiple of 1024 or exactly 0.5
		//if ((long)(size * 2) % 1024 != 0)
		//	final_size += 0.05;
			//ft_printf("%*f%c ", padding, final_size, letter);
		ft_printf("%*.1f%c ", padding, ft_centiceil(final_size), letter);
	}
	else
	{
		//if ((long)size % 1024 != 0)
		//	final_size += 1;
		ft_printf("%*ld%c ", padding, (long int)ft_ceil(final_size), letter);
	}
}

/*
**	Print file size shortened
*/

void	print_size_readable_si(off_t long_size, int padding, double divider)
{
	//	Padding - 1 because we add a letter to the output
	//padding--;
	char letter = ' ';
	double final_size = 0.0;
	double size = (double)long_size;
	if (size < divider)
	{
		ft_printf("%*ld ", padding + 1, (long int)size);
		return ;
	}
	else if (ft_ceil(size / divider) < divider)
	{
		final_size = size / divider;
		letter = 'k';
	}
	else if (ft_ceil(size / (divider * divider)) < divider)
	{
		final_size = size / (divider * divider);
		letter = 'M';
	}
	else if (ft_ceil(size / (divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider);
		letter = 'G';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider);
		letter = 'T';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider * divider)) < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider * divider);
		letter = 'P';
	}
	else if (ft_ceil(size / (divider * divider * divider * divider * divider * divider))
		 < divider)
	{
		final_size = size / (divider * divider * divider
			 * divider * divider * divider);
		letter = 'E';
	}
	else
	{
		final_size = size / (divider * divider * divider
			 * divider * divider * divider * divider);
		letter = '?';
	}
	int len = get_doublelen(final_size);
	if (len == 1 && letter != ' ')
	{
		//	Round up if not multiple of 1024 or exactly 0.5
		//if ((long)(size * 2) % 1024 != 0)
		//	final_size += 0.05;
			//ft_printf("%*f%c ", padding, final_size, letter);
		ft_printf("%*.1f%c ", padding, ft_centiceil(final_size), letter);
	}
	else
	{
		//if ((long)size % 1024 != 0)
		//	final_size += 1;
		ft_printf("%*ld%c ", padding, (long int)ft_ceil(final_size), letter);
	}
}


/*
**	Print file size
*/

void	print_size(off_t size, int padding, unsigned long long opt)
{
	if (opt & OPT_L || opt & OPT_G || opt & OPT_N)
	{
		if (opt & OPT_H)
			print_size_readable(size, padding, 1024);
		else if (opt & OPT_SI)
			print_size_readable_si(size, padding, 1000);
		else
			ft_printf("%*ld ", padding, size);
	}
	else
	{
		if (opt & OPT_H)
			ft_printf("%ld ", size);
		else
			ft_printf("%ld ", size);
	}
}
