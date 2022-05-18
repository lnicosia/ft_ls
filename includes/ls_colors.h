/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_colors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 18:54:26 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/18 18:58:35 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS_COLORS_H
#define LS_COLORS_H

enum
{
	RESET,
	DIR,
	LINK,
	MULTIHARADLINK,
	FIFO,
	SOCK,
	DOOR,
	BLK,
	CHR,
	ORPHAN,
	MISSING,
	SETUID,
	SETGID,
	CAPABILITY,
	STICK_OTHER_WRITABLE,
	OTHER_WRITABLE,
	STICKY,
	EXEC
};

#endif