/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 18:18:46 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/20 16:46:03 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "options.h"
#include "ls_colors.h"

void	print_ls_dir_colors(t_term_color dir_colors[MAX_DIR_COLORS])
{
	ft_printf("%s%s%sRESET{reset}\n", dir_colors[RESET].attribute,
	dir_colors[RESET].foreground, dir_colors[RESET].background);
	ft_printf("%s%s%sDIR{reset}\n", dir_colors[DIR].attribute,
	dir_colors[DIR].foreground, dir_colors[DIR].background);
	ft_printf("%s%s%sLINK{reset}\n", dir_colors[LINK].attribute,
	dir_colors[LINK].foreground, dir_colors[LINK].background);
	ft_printf("%s%s%sMULTIHARADLINK{reset}\n", dir_colors[MULTIHARADLINK].attribute,
	dir_colors[MULTIHARADLINK].foreground, dir_colors[MULTIHARADLINK].background);
	ft_printf("%s%s%sFIFO{reset}\n", dir_colors[FIFO].attribute,
	dir_colors[FIFO].foreground, dir_colors[FIFO].background);
	ft_printf("%s%s%sSOCK{reset}\n", dir_colors[SOCK].attribute,
	dir_colors[SOCK].foreground, dir_colors[SOCK].background);
	ft_printf("%s%s%sDOOR{reset}\n", dir_colors[DOOR].attribute,
	dir_colors[DOOR].foreground, dir_colors[DOOR].background);
	ft_printf("%s%s%sBLK{reset}\n", dir_colors[BLK].attribute,
	dir_colors[BLK].foreground, dir_colors[BLK].background);
	ft_printf("%s%s%sCHR{reset}\n", dir_colors[CHR].attribute,
	dir_colors[CHR].foreground, dir_colors[CHR].background);
	ft_printf("%s%s%sORPHAN{reset}\n", dir_colors[ORPHAN].attribute,
	dir_colors[ORPHAN].foreground, dir_colors[ORPHAN].background);
	ft_printf("%s%s%sMISSING{reset}\n", dir_colors[MISSING].attribute,
	dir_colors[MISSING].foreground, dir_colors[MISSING].background);
	ft_printf("%s%s%sSETUID{reset}\n", dir_colors[SETUID].attribute,
	dir_colors[SETUID].foreground, dir_colors[SETUID].background);
	ft_printf("%s%s%sSETGID{reset}\n", dir_colors[SETGID].attribute,
	dir_colors[SETGID].foreground, dir_colors[SETGID].background);
	ft_printf("%s%s%sCAPABILITY{reset}\n", dir_colors[CAPABILITY].attribute,
	dir_colors[CAPABILITY].foreground, dir_colors[CAPABILITY].background);
	ft_printf("%s%s%sSTICK_OTHER_WRITABLE{reset}\n", dir_colors[STICK_OTHER_WRITABLE].attribute,
	dir_colors[STICK_OTHER_WRITABLE].foreground, dir_colors[STICK_OTHER_WRITABLE].background);
	ft_printf("%s%s%sOTHER_WRITABLE{reset}\n", dir_colors[OTHER_WRITABLE].attribute,
	dir_colors[OTHER_WRITABLE].foreground, dir_colors[OTHER_WRITABLE].background);
	ft_printf("%s%s%sSTICKY{reset}\n", dir_colors[STICKY].attribute,
	dir_colors[STICKY].foreground, dir_colors[STICKY].background);
	ft_printf("%s%s%sEXEC{reset}\n", dir_colors[EXEC].attribute,
	dir_colors[EXEC].foreground, dir_colors[EXEC].background);
}

void	print_ls_extension_colors(t_extension_color extension_colors[MAX_EXTENSION_COLORS])
{
	for (size_t i = 0; i < MAX_DIR_COLORS; i++)
	{
		ft_printf("Extension = '%s%s%s%s{reset}'\n",
		extension_colors[i].term_color.attribute,
		extension_colors[i].term_color.foreground,
		extension_colors[i].term_color.background,
		extension_colors[i].extension);
	}
}

void	print_ls_colors(t_term_color dir_colors[MAX_DIR_COLORS],
	t_extension_color extension_colors[MAX_EXTENSION_COLORS])
{
	print_ls_dir_colors(dir_colors);
	print_ls_extension_colors(extension_colors);
}

void	set_term_color(char* str, t_term_color* term_color)
{
	size_t	i;
	int		color;

	if (!str)
		return;
	ft_bzero(term_color->attribute, COLOR_STRINGS_LEN);
	ft_strcpy(term_color->attribute, "\033[0m\0");
	i = 0;
	while (str[i])
	{
		color = ft_atoi(str + i);
		if (color <= 8)
		{
			term_color->attribute[2] = (char)(color + '0');
			term_color->attribute[3] = 'm';
			term_color->attribute[4] = '\0';
		}
		else if (color >= 30 && color <= 37)
		{
			ft_bzero(term_color->foreground, COLOR_STRINGS_LEN);
			ft_strcpy(term_color->foreground, "\033[");
			term_color->foreground[2] = (char)(color / 10 + '0');
			term_color->foreground[3] = (char)(color % 10 + '0');
			term_color->foreground[4] = 'm';
			term_color->foreground[5] = '\0';
		}
		else if (color >= 40 && color <= 47)
		{
			ft_bzero(term_color->background, COLOR_STRINGS_LEN);
			ft_strcpy(term_color->background, "\033[");
			term_color->background[2] = (char)(color / 10 + '0');
			term_color->background[3] = (char)(color % 10 + '0');
			term_color->background[4] = 'm';
			term_color->background[5] = '\0';
		}
		while (str[i] && str[i] != ';')
			i++;
		i++;
	}
}

void	retrieve_ls_colors(t_term_color dir_colors[MAX_DIR_COLORS],
	t_extension_color extension_colors[MAX_EXTENSION_COLORS])
{
	char*	env_str;
	char**	colors;
	size_t	extension_index;
	size_t	i;

	env_str = getenv("LS_COLORS");
	if (env_str == NULL)
		return ;
	colors = ft_strsplit(env_str, ':');
	if (colors == NULL)
		return ;
	i = 0;
	extension_index = 0;
	while (colors[i])
	{
		size_t	entry_len = ft_strlen(colors[i]);
		if (ft_strnequ(colors[i], "*.", 2)
			&& extension_index < MAX_EXTENSION_COLORS)
		{
			//	Get extension
			//	We take j + 2 for colors[i] to skip the "*."
			size_t j = 0;
			while (j + 2 < entry_len && j < EXTENSION_MAX_LEN && colors[i][j + 2] != '=')
			{
				extension_colors[extension_index].extension[j] =
					colors[i][j + 2];
				j++;
			}
			extension_colors[extension_index].extension[j] = '\0';
			//	Get colors
			set_term_color(colors[i] + j + 3, &(extension_colors[extension_index].term_color));
			extension_index++;
		}
		else if (ft_strnequ(colors[i], "rs", 2))
			set_term_color(colors[i] + 3, &dir_colors[RESET]);
		else if (ft_strnequ(colors[i], "di", 2))
			set_term_color(colors[i] + 3, &dir_colors[DIR]);
		else if (ft_strnequ(colors[i], "ln", 2))
			set_term_color(colors[i] + 3, &dir_colors[LINK]);
		else if (ft_strnequ(colors[i], "mh", 2))
			set_term_color(colors[i] + 3, &dir_colors[MULTIHARADLINK]);
		else if (ft_strnequ(colors[i], "pi", 2))
			set_term_color(colors[i] + 3, &dir_colors[FIFO]);
		else if (ft_strnequ(colors[i], "so", 2))
			set_term_color(colors[i] + 3, &dir_colors[SOCK]);
		else if (ft_strnequ(colors[i], "do", 2))
			set_term_color(colors[i] + 3, &dir_colors[DOOR]);
		else if (ft_strnequ(colors[i], "bd", 2))
			set_term_color(colors[i] + 3, &dir_colors[BLK]);
		else if (ft_strnequ(colors[i], "cd", 2))
			set_term_color(colors[i] + 3, &dir_colors[CHR]);
		else if (ft_strnequ(colors[i], "or", 2))
			set_term_color(colors[i] + 3, &dir_colors[ORPHAN]);
		else if (ft_strnequ(colors[i], "mi", 2))
			set_term_color(colors[i] + 3, &dir_colors[MISSING]);
		else if (ft_strnequ(colors[i], "su", 2))
			set_term_color(colors[i] + 3, &dir_colors[SETUID]);
		else if (ft_strnequ(colors[i], "sg", 2))
			set_term_color(colors[i] + 3, &dir_colors[SETGID]);
		else if (ft_strnequ(colors[i], "ca", 2))
			set_term_color(colors[i] + 3, &dir_colors[CAPABILITY]);
		else if (ft_strnequ(colors[i], "tw", 2))
			set_term_color(colors[i] + 3, &dir_colors[STICK_OTHER_WRITABLE]);
		else if (ft_strnequ(colors[i], "ow", 2))
			set_term_color(colors[i] + 3, &dir_colors[OTHER_WRITABLE]);
		else if (ft_strnequ(colors[i], "st", 2))
			set_term_color(colors[i] + 3, &dir_colors[STICKY]);
		else if (ft_strnequ(colors[i], "ex", 2))
			set_term_color(colors[i] + 3, &dir_colors[EXEC]);
		ft_strdel(&colors[i]);
		i++;
	}
	free(colors);
}

char*	get_extension(char* file)
{
	if (file == NULL)
		return NULL;
	size_t len = ft_strlen(file);
	size_t i = len - 1;
	while (i > 0 && file[i] && file[i] != '.')
		i--;
	if (i + 1 < len)
		return (file + i + 1);
	return (file);
}

/*
**	Sets the right color according to the type of a file
*/

void	set_color(char *file, mode_t mode, t_stat stats)
{
	static t_term_color			dir_colors[MAX_DIR_COLORS];
	static t_extension_color	extension_colors[MAX_EXTENSION_COLORS];
	static int					first_call = 1;

	if (first_call == 1)
	{
		retrieve_ls_colors(dir_colors, extension_colors);
		first_call = 0;
	}
	ft_printf("\nChecking file %s\n", file);
	if (S_ISREG(mode))
	{
		for (size_t i = 0; i < MAX_EXTENSION_COLORS; i++)
		{
			if (ft_strequ(get_extension(file), extension_colors[i].extension))
				ft_printf("%s%s%s",
				extension_colors[i].term_color.attribute,
				extension_colors[i].term_color.foreground,
				extension_colors[i].term_color.background);
		}
	}

	if (S_ISDIR(mode))
		ft_printf("%s%s%s", dir_colors[DIR].attribute,
		dir_colors[DIR].foreground, dir_colors[DIR].background);
	if (S_ISLNK(mode))
	{
		ft_printf("File is link\n");
		ft_printf("%s%s%s", dir_colors[LINK].attribute,
		dir_colors[LINK].foreground, dir_colors[LINK].background);
	}
	if (S_ISREG(mode) && stats.st_nlink > 1)
		ft_printf("%s%s%s", dir_colors[MULTIHARADLINK].attribute,
		dir_colors[MULTIHARADLINK].foreground, dir_colors[MULTIHARADLINK].background);
	if (S_ISFIFO(mode))
		ft_printf("%s%s%s", dir_colors[FIFO].attribute,
		dir_colors[FIFO].foreground, dir_colors[FIFO].background);
	if (S_ISSOCK(mode))
		ft_printf("%s%s%s", dir_colors[SOCK].attribute,
		dir_colors[SOCK].foreground, dir_colors[SOCK].background);
	if (S_ISBLK(mode))
		ft_printf("%s%s%s", dir_colors[BLK].attribute,
		dir_colors[BLK].foreground, dir_colors[BLK].background);
	if (S_ISCHR(mode))
		ft_printf("%s%s%s", dir_colors[CHR].attribute,
		dir_colors[CHR].foreground, dir_colors[CHR].background);
	if (S_ISLNK(mode) && !is_link_valid(file))
	{
		ft_printf("No print link %s\n", file);
		ft_printf("%s%s%s", dir_colors[ORPHAN].attribute,
		dir_colors[ORPHAN].foreground, dir_colors[ORPHAN].background);
	}
	if (S_ISLNK(mode) && !should_print_link(file))
	{
		ft_printf("Invalid link %s\n", file);
		ft_printf("%s%s%s", dir_colors[MISSING].attribute,
		dir_colors[MISSING].foreground, dir_colors[MISSING].background);
	}
	if (S_ISREG(mode) && mode & S_ISGID)
		ft_printf("%s%s%s", dir_colors[SETGID].attribute,
		dir_colors[SETGID].foreground, dir_colors[SETGID].background);
	if (S_ISREG(mode) && mode & S_ISUID)
		ft_printf("%s%s%s", dir_colors[SETUID].attribute,
		dir_colors[SETUID].foreground, dir_colors[SETUID].background);
	if (S_ISDIR(mode) && mode & S_IWOTH && mode & S_ISVTX)
		ft_printf("%s%s%s", dir_colors[STICK_OTHER_WRITABLE].attribute,
		dir_colors[STICK_OTHER_WRITABLE].foreground, dir_colors[STICK_OTHER_WRITABLE].background);
	if (S_ISDIR(mode) && mode & S_IWOTH && !(mode & S_ISVTX))
		ft_printf("%s%s%s", dir_colors[OTHER_WRITABLE].attribute,
		dir_colors[OTHER_WRITABLE].foreground, dir_colors[OTHER_WRITABLE].background);
	if (S_ISDIR(mode) && !(mode & S_IWOTH) && mode & S_ISVTX)
		ft_printf("%s%s%s", dir_colors[STICKY].attribute,
		dir_colors[STICKY].foreground, dir_colors[STICKY].background);
	if (S_ISREG(mode) && mode & S_IXUSR && mode & S_IXGRP && mode & S_IXOTH)
		ft_printf("%s%s%s", dir_colors[EXEC].attribute,
		dir_colors[EXEC].foreground, dir_colors[EXEC].background);

}
