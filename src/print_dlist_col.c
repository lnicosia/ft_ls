/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dlist_col.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:21:45 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 16:42:22 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "libft.h"

int		print_dlist_col(t_dlist *lst, size_t len, unsigned short winsize,
int opt)
{
	(void)lst;
	(void)len;
	(void)winsize;
	(void)opt;
	ft_printf("Len = %ld\n", len);
	ft_printf("Window size = %d\n", winsize);
	return (0);
}