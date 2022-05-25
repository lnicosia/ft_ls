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
# define LS_COLORS_H

# define MAX_DIR_COLORS			100
# define MAX_EXTENSION_COLORS	1000
# define COLOR_STRINGS_LEN		6
# define EXTENSION_MAX_LEN		10

typedef	struct		s_term_color
{
	char			attribute[COLOR_STRINGS_LEN];
	char			foreground[COLOR_STRINGS_LEN];
	char			background[COLOR_STRINGS_LEN];
}					t_term_color;

typedef struct		s_extension_color
{
	char			extension[EXTENSION_MAX_LEN];
	t_term_color	term_color;
}					t_extension_color;

enum
{
	RESET,
	DIR_COLOR,
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

void	retrieve_ls_colors(t_term_color dir_colors[MAX_DIR_COLORS],
	t_extension_color extension_colors[MAX_EXTENSION_COLORS]);
void	print_ls_colors(t_term_color dir_colors[MAX_DIR_COLORS],
	t_extension_color extension_colors[MAX_EXTENSION_COLORS]);
void	print_ls_extension_colors(
	t_extension_color extension_colors[MAX_EXTENSION_COLORS]);
void	print_ls_dir_colors(t_term_color dir_colors[MAX_DIR_COLORS]);

#endif
