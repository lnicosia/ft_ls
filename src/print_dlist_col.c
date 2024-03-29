/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dlist_col.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:21:45 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 19:22:32 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "libft.h"
#include "options.h"
#include <math.h>

size_t	get_col_padding(t_file* files, size_t array_len, size_t nb_lines,
	size_t current_file, size_t winsize, unsigned long long opt)
{
	size_t	i;
	size_t	padding;
	size_t	strlen;
	size_t	special_char_padding;
	int		special_chars;
	char	*name;

	strlen = 0;
	padding = 0;
	i = 0;
	while (i < nb_lines && current_file < array_len)
	{
		if (!(name = ft_strrchr(files[current_file].name, '/')))
			name = files[current_file].name;
		else
			name++;
		special_char_padding = 0;
		if (isatty(STDOUT_FILENO))
		{
			special_chars = contains_special_chars(name);
			if (special_chars == 1 || special_chars == 2)
			{
				if (!(opt & OPT_NCAPS))
					special_char_padding += 1;
			}
			else if (special_chars == 3)
			{
				if (!(opt & OPT_NCAPS))
				{
					//if (opt & OPT_P)
					//	special_char_padding += 6;
					//else
						special_char_padding += 7;
				}
			}
			if (opt & OPT_SPECIAL_CHAR)
				special_char_padding++;
		}
		if (opt & OPT_P && S_ISDIR(files[current_file].stats.st_mode))
			special_char_padding++;
		if ((strlen = ft_strlen(name) + 2 + special_char_padding) > padding && strlen < winsize)
			padding = strlen;
		i++;
		current_file++;
	}
	return (padding);
}

int		print_n_lines(t_file* files, size_t array_len, size_t nb_lines,
	size_t winsize, unsigned long long opt)
{
	size_t	nb_col;
	size_t	lstlen;

	lstlen = array_len;
	nb_col = (size_t)ceil((double)lstlen / (double)nb_lines);
	for (size_t line = 0; line < nb_lines; line++)
	{
		for (size_t current_col = 0; current_col < nb_col; current_col++)
		{
			size_t col_padding = get_col_padding(files, array_len, nb_lines,
				current_col * nb_lines, winsize, opt);
			size_t index = current_col * nb_lines + line;
			size_t next_index = (current_col + 1) * nb_lines + line;
			if (index < array_len)
			{
				if (current_col == nb_col - 1 || next_index >= array_len)
				{
					print_file_name(files[index].stats, &files[index],
						0, opt);
					ft_bprintf(0, "\n");
				}
				else
					print_file_name(files[index].stats, &files[index],
						col_padding, opt);
			}
		}
	}
	if (lstlen != 0)
		return (-1);
	return (0);
}

int		preprint_n_lines(t_file* files, size_t array_len, size_t nb_lines, size_t winsize,
	unsigned long long opt)
{
	size_t	nb_col;

	nb_col = (size_t)ceil((double)array_len / (double)nb_lines);
	size_t printed_chars = 0;
	for (size_t current_col = 0; current_col < nb_col; current_col++)
	{
		size_t col_padding = get_col_padding(files, array_len, nb_lines,
			current_col * nb_lines, winsize, opt);
		printed_chars += col_padding;
	}
	if (printed_chars > winsize + 1)
		return (1);
	return (0);
}

int		print_dlist_col(t_file* files, size_t array_len, size_t len,
	unsigned short winsize, unsigned long long opt)
{
	size_t	nb_lines;

	nb_lines = (size_t)ceil((double)len / winsize);
	while (preprint_n_lines(files, array_len, nb_lines, winsize, opt))
	{
		nb_lines++;
	}
	print_n_lines(files, array_len, nb_lines, winsize, opt);
	return (0);
}
