/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 11:06:12 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 12:13:22 by lnicosia         ###   ########.fr       */
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
	return (1);
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
	return (((t_file*)s1)->stats.st_mtime < ((t_file*)s2)->stats.st_mtime);
}

/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_atimes(void *s1, void *s2)
{
	return (((t_file*)s1)->stats.st_atime < ((t_file*)s2)->stats.st_atime);
}

/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_ctimes(void *s1, void *s2)
{
	return (((t_file*)s1)->stats.st_ctime < ((t_file*)s2)->stats.st_ctime);
}