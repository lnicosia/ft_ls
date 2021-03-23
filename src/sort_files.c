/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:40:06 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/23 18:54:49 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Merging part of the algorithm
*/

int		merge(t_list *left, t_list *right, t_list **res)
{
	*res = NULL;
	ft_printf("Merging\n");
	while (left && right)
	{
		if (ft_strcmp((char*)left->content, (char*)right->content) <= 0)
		{
			ft_lstpushback(res, left);
			left = left->next;
		}
		else
		{
			ft_lstpushback(res, right);
			right = right->next;
		}
	}
	ft_printf("Emptying\n");
	while (left)
	{
		ft_lstpushback(res, left);
		left = left->next;
	}
	while (right)
	{
		ft_lstpushback(res, right);
		right = right->next;
	}
	return (0);
}

/*
**	Sort our list using a merge-sort algorithm
*/

int		sort_files(t_list **lst)
{
	t_list	*left;
	t_list	*right;
	t_list	*tmp;
	t_list	*res;
	int		len;
	int		i;

	ft_printf("Lst len = %d\n", ft_lstlen(*lst));
	if ((len = ft_lstlen(*lst)) <= 1)
		return (0);
	ft_bzero(&left, sizeof(left));
	ft_bzero(&right, sizeof(right));
	res = NULL;
	tmp = *lst;
	i = 0;
	while (i < len)
	{
		ft_printf("i = %d\n", i);
		if (i < len / 2)
		{
			ft_printf("Left\n");
			ft_lstpushback(&left, tmp);
		}
		else
		{
			ft_printf("Right\n");
			ft_lstpushback(&right, tmp);
		}
		tmp = tmp->next;
		i++;
	}
	ft_printf("Sorting left\n");
	sort_files(&left);
	ft_printf("Sorting right\n");
	sort_files(&right);
	merge(left, right, &res);
	return (0);
}