/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:40:06 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/24 12:46:32 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

/*
**	Sort our list using a quick sort algorithm
*/

int		sort_files(t_list **lst, int start, int end)
{
	int		i;
	//t_list	*tmp;
	t_list	*pivot;

	if (end - start <= 1)
		return (0);
	pivot = *lst;
	i = 0;
	while (i < floor((end + start) / 2) - 1)
	{
		i++;
		pivot = pivot->next;
	}
	i = start;
	return (0);
}