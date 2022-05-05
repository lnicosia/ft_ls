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

int				get_snblen(long int nb)
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

t_ls_padding	get_padding(t_dlist *dlst, blksize_t *dir_size)
{
	t_ls_padding		padding;
	int					len;
	long int			size;
	long unsigned int	usize;

	ft_bzero(&padding, sizeof(padding));
	while (dlst)
	{
		size = get_block_size(((t_file*)(dlst->content)));
		(*dir_size) += size;
		len = get_snblen(((t_file*)(dlst->content))->stats.st_size);
		if (len > padding.size)
			padding.size = len;
		usize = ((t_file*)(dlst->content))->stats.st_nlink;
		len = get_nblen(usize);
		if (len > padding.links)
			padding.links = len;
		len = get_userlen((t_file*)dlst->content);
		if (len > padding.user)
			padding.user = len;
		len = get_grouplen((t_file*)dlst->content);
		if (len > padding.group)
			padding.group = len;
		dlst = dlst->next;
	}
	return (padding);
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
		return ;
	first = 1;
	while (dlst && dlst->prev)
		dlst = dlst->prev;
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G)
	{
		padding = get_padding(dlst, &dir_size);
		double size = (double)dir_size / 1024.0 + 0.5;
		if (opt & OPT_TOTAL)
			ft_printf("total %ld\n", (long int)size);
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
		return ;
	first = 1;
	while (dlst && dlst->next)
		dlst = dlst->next;
	dir_size = 0;
	ft_bzero(&padding, sizeof(padding));
	if (opt & OPT_L || opt & OPT_G)
	{
		padding = get_padding(dlst, &dir_size);
		double size = (double)dir_size / 1024.0 + 0.5;
		if (opt & OPT_TOTAL)
			ft_printf("total %ld\n", (long int)size);
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
