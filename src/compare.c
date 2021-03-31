/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 11:06:12 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/31 15:05:52 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"

/*
**	Function to not sort
*/

int		compare_none(void *s1, void *s2)
{
	(void)s1;
	(void)s2;
	return (0);
}

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		compare_names(void *s1, void *s2)
{
	return (ft_strcmp(((t_file*)s1)->name, ((t_file*)s2)->name));
}

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		compare_names_no_case(void *s1, void *s2)
{
	return (ft_stricmp(((t_file*)s1)->name, ((t_file*)s2)->name));
}

/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_times(void *s1, void *s2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file*)s1;
	f2 = (t_file*)s2;
	if (f1->stats.st_mtime == f2->stats.st_mtime)
	{
		if (f1->stats.st_mtim.tv_nsec == f2->stats.st_mtim.tv_nsec)
			return (compare_names(s1, s2));
		return (f1->stats.st_mtim.tv_nsec < f2->stats.st_mtim.tv_nsec);
	}
	return (f1->stats.st_mtime < f2->stats.st_mtime);
}

/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_atimes(void *s1, void *s2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file*)s1;
	f2 = (t_file*)s2;
	if (f1->stats.st_atime == f2->stats.st_atime)
	{
		if (f1->stats.st_atim.tv_nsec == f2->stats.st_atim.tv_nsec)
			return (compare_names(s1, s2));
		return (f1->stats.st_atim.tv_nsec < f2->stats.st_atim.tv_nsec);
	}
	return (f1->stats.st_atime < f2->stats.st_atime);
}

/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_ctimes(void *s1, void *s2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file*)s1;
	f2 = (t_file*)s2;
	if (f1->stats.st_ctime == f2->stats.st_ctime)
	{
		if (f1->stats.st_ctim.tv_nsec == f2->stats.st_ctim.tv_nsec)
			return (compare_names(s1, s2));
		return (f1->stats.st_ctim.tv_nsec < f2->stats.st_ctim.tv_nsec);
	}
	return (f1->stats.st_ctime < f2->stats.st_ctime);
}