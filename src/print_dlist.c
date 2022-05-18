/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 18:47:26 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 15:10:39 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "libft.h"
#include "options.h"
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>

int				get_human_readable_nblen(long int nb, long int divider)
{
	int	len;

	int rest = 0;
	double dnb = (double)nb;
	double ddivider = (double)divider;
	//ft_printf("Nb = %d\n", nb);
	while (dnb > ddivider)
	{
		dnb /= ddivider;
		rest++;
	}
	len = 0;
	//ft_printf("Nb after divide = %f\n", dnb);
	if (dnb < 10)
		dnb = ft_centiceil(dnb);
	else
		dnb = ft_ceil(dnb);
	nb = (long int)dnb;
	//ft_printf("Nb after rounding = %f\n", dnb);
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	//	If the number was divided and is 1 digit long, there is a comma
	//	Add 2 to the len, one for the '.' and one digit
	if (len == 1 && rest >= 1)
	{
		len += 2;
	}
	//	If size < 1024, count one less len because there will be no letter
	if (rest == 0)
		len--;
	if (ft_ceil(dnb) >= divider)
	{
		len = 3;
		//ft_printf("Ceil overflows\n");
	}
	//ft_printf("Len = %d\n", len);
	return (len);
}

int				get_signed_nblen(long int nb)
{
	int	len;

	if (nb == 0)
		return (1);
	len = 0;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int				get_nblen(long unsigned int nb)
{
	int	len;

	if (nb == 0)
		return (1);
	len = 0;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int				get_userlen(t_file *file)
{
	t_passwd	*passwd;

	if (!(passwd = getpwuid(file->stats.st_uid)))
	{
		return (get_nblen(file->stats.st_uid));

	}
	return ((int)ft_strlen(passwd->pw_name));
}

int				get_grouplen(t_file *file)
{
	t_group	*group;

	if (!(group = getgrgid(file->stats.st_gid)))
	{
		return (get_nblen(file->stats.st_gid));
	}
	return ((int)ft_strlen(group->gr_name));
}

long int		get_block_size(t_file *file)
{
	long int	size;

	size = 0;
	size = file->stats.st_blocks * 512;
	return (size);
}

t_ls_padding	get_padding(t_file* files, size_t array_len, blksize_t *dir_size,
	unsigned long long opt)
{
	t_ls_padding		padding;
	int					len;
	long int			size;
	long unsigned int	usize;
	t_file				file;
	int					char_device;

	ft_bzero(&padding, sizeof(padding));
	char_device = 0;
	for (size_t i = 0; i < array_len; i++)
	{
		file = files[i];
		size = get_block_size(&file);
		(*dir_size) += size;
		if (S_ISCHR(file.stats.st_mode) || S_ISBLK(file.stats.st_mode))
		{
			char_device = 1;
			len = get_nblen(major(file.stats.st_rdev));
			if (len > padding.major_size)
				padding.major_size = len;
			len = get_nblen(minor(file.stats.st_rdev));
			if (len > padding.minor_size)
				padding.minor_size = len;
		}
		else if (opt & OPT_H)
			len = get_human_readable_nblen(file.stats.st_size, 1024);
		else if (opt & OPT_SI)
			len = get_human_readable_nblen(file.stats.st_size, 1000);
		else
			len = get_signed_nblen(file.stats.st_size);
		if (len > padding.size)
			padding.size = len;
		usize = file.stats.st_nlink;
		len = get_nblen(usize);
		if (len > padding.links)
			padding.links = len;
		if (opt & OPT_N)
			len = get_nblen(file.stats.st_uid);
		else
			len = get_userlen(&file);
		if (len > padding.user)
			padding.user = len;
		if (opt & OPT_N)
			len = get_nblen(file.stats.st_gid);
		else
			len = get_grouplen(&file);
		if (len > padding.group)
			padding.group = len;
		if (file.has_extended || file.has_acl)
			padding.xattr = 1;
	}
	if (char_device && padding.major_size + padding.minor_size + 2 > padding.size)
	{
		padding.size = padding.major_size + padding.minor_size + 2;
		if (opt & OPT_H)
			padding.size--;
	}
	return (padding);
}

void		print_total(long int long_size, unsigned long long opt)
{
	double size = (double)long_size;
	if (opt & OPT_H || opt & OPT_SI)
	{
		double divider = 1024;
		if (opt & OPT_H)
		 	divider = 1024;
		else if (opt & OPT_SI)
		 	divider = 1000;
		char letter = ' ';
		double final_size = 0.0;
		if (size < divider)
		{
			ft_printf("total %ld\n", (long int)size);
			return ;
		}
		else if (ft_ceil(size / divider) < divider)
		{
			final_size = size / divider;
			if (opt & OPT_H)
				letter = 'K';
			else if (opt & OPT_SI)
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
			ft_printf("total %.1f%c\n", ft_centiceil(final_size), letter);
		}
		else
		{
			final_size = ft_ceil(final_size);
			ft_printf("total %ld%c\n", (long int)(final_size), letter);
		}
	}
	else
		ft_printf("total %ld\n", (long int)(size / 1024.0 + 0.5));
}

/*
**	Print the list content
**	Assumes it's made of t_file*
*/

void			print_dlist(t_file* files, size_t array_len, unsigned short winsize,
	unsigned long long opt)
{
	t_ls_padding	padding;
	blksize_t		dir_size;
	int				len;

	if (!files)
		return ;
	len = 0;
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G || opt & OPT_N || opt & OPT_O)
	{
		padding = get_padding(files, array_len, &dir_size, opt);
		if (opt & OPT_TOTAL)
			print_total((long int)dir_size, opt);
	}
	for (size_t i = 0; i < array_len; i++)
	{
		t_file file = files[i];
		if (i != 0)
		{
			if (opt & OPT_M)
			{
				char* name;
				if (opt & OPT_PATH || !(name = ft_strrchr(file.name, '/')))
					name = file.name;
				else
					name++;
				if (len + 2 + (int)ft_strlen(name) < winsize)
					len += ft_printf(", ");
				else
				{
					ft_printf(",\n");
					len = 0;
				}
			}
			else if (opt & OPT_CCAPS)
				ft_printf("  ");
			else
				ft_printf("\n");
		}
		len += print_file(file, padding, opt);
	}
	ft_printf("\n");
}

/*
**	Print the list content
**	Assumes it's made of t_file*
*/

void			print_dlist_reverse(t_file* files, size_t array_len, unsigned short winsize,
	unsigned long long opt)
{
	t_ls_padding	padding;
	blksize_t		dir_size;
	int				len;

	if (!files)
	{
		if (opt & OPT_TOTAL)
			ft_printf("total 0\n");
		return ;
	}
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G || opt & OPT_N || opt & OPT_O)
	{
		padding = get_padding(files, array_len, &dir_size, opt);
		if (opt & OPT_TOTAL)
			print_total((long int)dir_size, opt);
	}
	for (size_t i = 0; i < array_len; i++)
	{
		t_file file = files[array_len - 1 - i];
		if (i != 0)
		{
			if (opt & OPT_M)
			{
				char* name;
				if (opt & OPT_PATH || !(name = ft_strrchr(file.name, '/')))
					name = file.name;
				else
					name++;
				if (len + 2 + (int)ft_strlen(name) < winsize)
					len += ft_printf(", ");
				else
				{
					ft_printf(",\n");
					len = 0;
				}
			}
			else if (opt & OPT_CCAPS)
				ft_printf("  ");
			else
				ft_printf("\n");
		}
		len += print_file(file, padding, opt);
	}
	ft_printf("\n");
}
