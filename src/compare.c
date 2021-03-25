/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 11:06:12 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 18:51:21 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"

/*
**	Function comparing the two file names that is going to be used
**	to go through our dlist
*/

int		compare_names(void *s1, void *s2)
{
	return (ft_strcmp(((t_file*)s1)->name, ((t_file*)s2)->name));
}


/*
**	Function comparing the two times that is going to be used
**	to go through our dlist
*/

int		compare_times(void *s1, void *s2)
{
	return (((t_file*)s1)->stats.st_mtime < ((t_file*)s2)->stats.st_mtime);
}