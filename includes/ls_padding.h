/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_padding.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 18:10:57 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/09 15:46:02 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS_PADDING_H
# define LS_PADDING_H

typedef struct	s_ls_padding
{
	int			links;
	int			user;
	int			group;
	int			size;
	int			major_size;
	int			minor_size;
	int			xattr;
}				t_ls_padding;


#endif
