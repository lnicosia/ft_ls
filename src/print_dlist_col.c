/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dlist_col.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:21:45 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/01 18:26:06 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "libft.h"
#include <math.h>

size_t	get_col_padding(t_dlist *lst, size_t nb_lines, size_t winsize)
{
	size_t	i;
	size_t	padding;
	size_t	strlen;
	char	*name;

	strlen = 0;
	padding = 0;
	i = 0;
	while (i < nb_lines && lst)
	{
		if (!(name = ft_strrchr(((t_file*)lst->content)->name, '/')))
			name = ((t_file*)lst->content)->name;
		else
			name++;
		if ((strlen = ft_strlen(name) + 2) > padding && strlen < winsize)
			padding = strlen;
		lst = lst->next;
		i++;
	}
	return (padding);
}

size_t	print_current_file(t_dlist *lst, size_t current_col,
size_t current_line, size_t nb_lines, size_t place_left, size_t winsize,
size_t *len, unsigned long long opt)
{
	size_t	i;
	size_t	j;
	size_t	padding;
	t_stat	stats;

	i = 0;
	while (i < current_col)
	{
		j = 0;
		while (j < nb_lines && lst->next)
		{
			lst = lst->next;
			j++;
		}
		i++;
	}
	padding = get_col_padding(lst, nb_lines, winsize);
	i = 0;
	while (i < current_line && lst->next)
	{
		lst = lst->next;
		i++;
	}
	if (i < current_line)
		return (place_left);
	if (lstat(((t_file*)lst->content)->name, &stats))
	{
		custom_error("ft_ls: cannot access '%s': ", ((t_file*)lst->content)->name);
		ft_perror("");
		return (padding);
	}
	if (padding <= place_left || (padding > winsize && current_col == 0))
	{
		print_file_name(stats, ((t_file*)lst->content)->name, padding, opt);
		(*len)--;
	}
	return (padding);
}

int		print_n_lines(t_dlist *lst, size_t nb_lines, size_t winsize, unsigned long long opt)
{
	size_t	nb_col;
	size_t	lstlen;
	size_t	i;
	size_t	printed_char;
	size_t	current_col;

	//ft_printf("Trying to print %d height\n", nb_lines);
	while (lst && lst->prev)
		lst = lst->prev;
	lstlen = ft_dlstlen(lst) + 1;
	nb_col = (size_t)ceil((double)lstlen / (double)nb_lines);
	i = 0;
	//ft_printf("There will be %d names per line\n", nb_col);
	while (i < nb_lines)
	{
		printed_char = 0;
		current_col = 0;
		while (printed_char < winsize && current_col < nb_col)
		{
			printed_char += print_current_file(lst, current_col, i, nb_lines,
			winsize - printed_char, winsize, &lstlen, opt);
			if (printed_char < winsize)
			{
				current_col++;
			}
		}
		ft_printf("\n");
		i++;
	}
	//ft_printf("\n{red}lstlen = %d\n{reset}", lstlen);
	if (lstlen != 0)
		return (-1);
	return (0);
}

size_t	preprint_current_file(t_dlist *lst, size_t current_col,
size_t current_line, size_t nb_lines, size_t place_left, size_t winsize,
size_t *len, unsigned long long opt)
{
	size_t	i;
	size_t	j;
	size_t	padding;

	(void)opt;
	i = 0;
	while (i < current_col)
	{
		j = 0;
		while (j < nb_lines && lst->next)
		{
			lst = lst->next;
			j++;
		}
		i++;
	}
	padding = get_col_padding(lst, nb_lines, winsize);
	i = 0;
	while (i < current_line && lst->next)
	{
		lst = lst->next;
		i++;
	}
	if (i < current_line)
		return (place_left);
	if (padding <= place_left || (padding > winsize && current_col == 0))
	{
		(*len)--;
	}
	return (padding);
}

int		preprint_n_lines(t_dlist *lst, size_t nb_lines, size_t winsize, unsigned long long opt)
{
	size_t	nb_col;
	size_t	lstlen;
	size_t	i;
	size_t	printed_char;
	size_t	current_col;

	(void)opt;
	while (lst && lst->prev)
		lst = lst->prev;
	lstlen = ft_dlstlen(lst) + 1;
	nb_col = (size_t)ceil((double)lstlen / (double)nb_lines);
	i = 0;
	//ft_printf("\n{green}Nb lines = %d\n{reset}", nb_lines);
	//ft_printf("\n{green}Nb col = %d\n{reset}", nb_col);
	//ft_printf("\n{green}lstlen = %d\n{reset}", lstlen);
	while (i < nb_lines)
	{
		printed_char = 0;
		current_col = 0;
		while (printed_char <= winsize && current_col < nb_col)
		{
			printed_char += preprint_current_file(lst, current_col, i, nb_lines,
			winsize - printed_char, winsize, &lstlen, opt);
			if (printed_char <= winsize)
			{
				current_col++;
			}
		}
		i++;
	}
	//ft_printf("\n{red}lstlen = %d\n{reset}", lstlen);
	if (lstlen != 0)
		return (1);
	return (0);
}

int		print_dlist_col(t_dlist *lst, size_t len, unsigned short winsize,
unsigned long long opt)
{
	size_t	nb_lines;

	//ft_printf("Len = %d\n", len);
	nb_lines = (size_t)ceil((double)len / winsize);
	while (preprint_n_lines(lst, nb_lines, winsize, opt))
	{
		nb_lines++;
	}
	//ft_printf("nb lines = %d\n", nb_lines);
	//ft_printf("winsize = %d\n", winsize);
	print_n_lines(lst, nb_lines, winsize, opt);
	return (0);
}
