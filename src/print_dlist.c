/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 18:47:26 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/05 14:40:43 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "libft.h"
#include "options.h"
#include <sys/stat.h>
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
	dnb += 0.1;
	nb = (long int)dnb;
	//ft_printf("Nb after rounding = %f\n", nb);
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
	//ft_printf("Len = %d\n", len);
	return (len);
}

int				get_signed_nblen(long int nb)
{
	int	len;

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
		ft_perror("Error: ");
		return (0);
	}
	return ((int)ft_strlen(passwd->pw_name));
}

int				get_grouplen(t_file *file)
{
	t_group	*group;

	if (!(group = getgrgid(file->stats.st_gid)))
	{
		ft_perror("Error: ");
		return (0);
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

t_ls_padding	get_padding(t_dlist *dlst, blksize_t *dir_size, int opt)
{
	t_ls_padding		padding;
	int					len;
	long int			size;
	long unsigned int	usize;
	t_file*				file;

	ft_bzero(&padding, sizeof(padding));
	while (dlst)
	{
		file = (t_file*)(dlst->content);
		size = get_block_size(file);
		(*dir_size) += size;
		if (opt & OPT_H)
			len = get_human_readable_nblen(file->stats.st_size, 1024);
		else
			len = get_signed_nblen(file->stats.st_size);
		if (len > padding.size)
			padding.size = len;
		usize = file->stats.st_nlink;
		len = get_nblen(usize);
		if (len > padding.links)
			padding.links = len;
		len = get_userlen(file);
		if (len > padding.user)
			padding.user = len;
		len = get_grouplen(file);
		if (len > padding.group)
			padding.group = len;
		dlst = dlst->next;
	}
	//ft_printf("Final len = %d\n", padding.size);
	return (padding);
}

void			print_total(long int size, int opt)
{
	if (opt & OPT_H)
	{
		long int divider = 1024;
		char letter = ' ';
		double final_size = 0.0;
		if (size < divider)
		{
			ft_printf("total %ld\n", size);
			return ;
		}
		else if (size < divider * divider)
		{
			final_size = (double)size / (double)divider;
			letter = 'K';
		}
		else if (size < divider * divider * divider)
		{
			final_size = (double)size / (double)(divider * divider);
			letter = 'M';
		}
		else if (size < divider * divider * divider * divider)
		{
			final_size = (double)size / (double)(divider * divider * divider);
			letter = 'G';
		}
		else if (size < divider * divider * divider * divider * divider)
		{
			final_size = (double)size / (double)(divider * divider * divider
				 * divider);
			letter = 'T';
		}
		else if (size < divider * divider * divider * divider * divider * divider)
		{
			final_size = (double)size / (double)(divider * divider * divider
				 * divider * divider);
			letter = 'P';
		}
		else if (size < divider * divider * divider * divider * divider * divider
			 * divider)
		{
			final_size = (double)size / (double)(divider * divider * divider
				 * divider * divider * divider);
			letter = 'E';
		}
		else
		{
			final_size = (double)size / (double)(divider * divider * divider
				 * divider * divider * divider * divider);
			letter = '?';
		}
		int len = get_doublelen(final_size);
		//ft_printf("final size = %f\n", final_size);
		if (len == 1 && letter != ' ')
		{
			ft_printf("total %.1f%c\n", ft_centiceil(final_size), letter);
		}
		else
		{
			//if ((long)size % 1024 != 0)
			//	final_size += 1;
			final_size = ft_ceil(final_size);
			ft_printf("total %ld%c\n", (long int)(final_size), letter);
		}
	}
	else
		ft_printf("total %ld\n", (long int)((double)size / 1024.0 + 0.5));
}

/*
**	Print the list content
**	Assumes it's made of t_file*
*/

void			print_dlist(t_dlist *dlst, int opt)
{
	int				first;
	t_ls_padding	padding;
	blksize_t		dir_size;

	if (!dlst)
	{
		if (opt & OPT_TOTAL)
			ft_printf("total 0\n");
		return ;
	}
	first = 1;
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G)
	{
		padding = get_padding(dlst, &dir_size, opt);
		if (opt & OPT_TOTAL)
			print_total((long int)dir_size, opt);
	}
	while (dlst)
	{
		if (first)
			first = 0;
		else if (!(opt & OPT_L || opt & OPT_G) && isatty(STDOUT_FILENO))
		{
			if (opt & OPT_CCAPS)
				ft_printf("  ");
			else
				ft_printf("\n");
		}
		print_file(((t_file*)dlst->content)->stats,
		((t_file*)dlst->content)->name, padding, opt);
		dlst = dlst->next;
	}
	if (!(opt & OPT_L || opt & OPT_G) && isatty(STDOUT_FILENO))
		ft_printf("\n");
}

/*
**	Print the list content
**	Assumes it's made of t_file*
*/

void			print_dlist_reverse(t_dlist *dlst, int opt)
{
	int				first;
	t_ls_padding	padding;
	blksize_t		dir_size;

	if (!dlst)
	{
		if (opt & OPT_TOTAL)
			ft_printf("total 0\n");
		return ;
	}
	first = 1;
	while (dlst && dlst->next)
		dlst = dlst->next;
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G)
	{
		padding = get_padding(dlst, &dir_size, opt);
		if (opt & OPT_TOTAL)
			print_total((long int)dir_size, opt);
	}
	while (dlst)
	{
		if (first)
			first = 0;
		else if (!(opt & OPT_L || opt & OPT_G) && isatty(STDOUT_FILENO))
		{
			if (opt & OPT_CCAPS)
				ft_printf("  ");
			else
				ft_printf("\n");
		}
		print_file(((t_file*)dlst->content)->stats,
		((t_file*)dlst->content)->name, padding, opt);
		dlst = dlst->prev;
	}
	if (!(opt & OPT_L) && isatty(STDOUT_FILENO))
		ft_printf("\n");
}
