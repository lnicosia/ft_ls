/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 11:06:12 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/25 11:36:53 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Function comparing the two string that is going to be use
**	to go through our dlist
*/

int		compare(void *s1, void *s2)
{
	return (ft_strcmp((const char*)s1, (const char*)s2));
}