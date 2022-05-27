/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_option_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 11:06:42 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/18 16:46:12 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "libft.h"
#include "ls_colors.h"

int    print_usage_stdin(void)
{
	ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
	ft_printf("List information about the FILEs (the current directory by default).\n");
	ft_printf("Sort entries alphabetically if none of -cftuvSUX nor --sort is specified\n");
	ft_printf("\n");
	//
	ft_printf("Mandatory arguments to long options are mandatory for short options too.\n");
	ft_printf("%4s%-25s%s", "", "  --acl", "with -l: if present, show ACL attributes\n");
	ft_printf("%4s%-25s%s", "-a", ", --all", "do not ignore entries starting with .\n");
	ft_printf("%4s%-25s%s", "-A", ", --almost-all", "do no list implied . and ..\n");
	ft_printf("%4s%-25s%s", "", "  --author", "with -l, print the author of each file\n");
	ft_printf("%4s%-25s%s", "-c", "", "with -lt: sort by, and show, ctime (time of last\n");
	ft_printf("%4s%-25s%s", "", "", "  modification of file status information);\n");
	ft_printf("%4s%-25s%s", "", "", "  with -l: show ctime and sort by name;\n");
	ft_printf("%4s%-25s%s", "", "", "  otherwise: sort by ctime, newest first\n");
	ft_printf("%4s%-25s%s", "-C", "", "list entries by column\n");
	ft_printf("%4s%-25s%s", "-d", ", --directory", "list directories themselves, not their contents\n");
	ft_printf("%4s%-25s%s", "", "  --dircolors", "show both file type and extension colors of the current env and exit\n");
	ft_printf("%4s%-25s%s", "-e", ", --extended", "with -l: if present, show extended attributes\n");
	ft_printf("%4s%-25s%s", "", ", --extension-colors", "show extension colors of the current env and exit\n");
	ft_printf("%4s%-25s%s", "-f", "", "do not sort, enable -aU, disable -ls --color\n");
	ft_printf("%4s%-25s%s", "", ", --filecolors", "show file type colors of the current env and exit\n");
	ft_printf("%4s%-25s%s", "-g", "", "like -l, but do not list owner\n");
	ft_printf("%4s%-25s%s", "-G", ", --color", "colorize the output\n");
	ft_printf("%4s%-25s%s", "-h", ", --human-readable", "with -l, print sizes like 1K 234M 2G etc.\n");
	ft_printf("%4s%-25s%s", "", "  --si", "likewise, but use powers of 1000 not 1024\n");
	ft_printf("%4s%-25s%s", "-l", "", "use a long listing format\n");
	ft_printf("%4s%-25s%s", "-m", "", "fill width with a comma separated list of entries\n");
	ft_printf("%4s%-25s%s", "-n", ", --numeric-uid-gid", "like -l, but list numeric user and group IDs\n");
	ft_printf("%4s%-25s%s", "-N", ", --literal", "print entry names without quoting\n");
	ft_printf("%4s%-25s%s", "-o", "", "like -l, but do not list group information\n");
	ft_printf("%4s%-25s%s", "-p", ", --indicator-style=slash", "\n");
	ft_printf("%4s%-25s%s", "", "", "append / indicator to directories\n");
	ft_printf("%4s%-25s%s", "-r", ", --reverse", "reverse order while sorting\n");
	ft_printf("%4s%-25s%s", "-R", ", --recursive", "list subdirectories recursively\n");
	ft_printf("%4s%-25s%s", "-t", "", "sort by modification time, newest first\n");
	ft_printf("%4s%-25s%s", "-T", "", "with -l, print the full timestamp\n");
	ft_printf("%4s%-25s%s", "-u", "", "with -lt: sort by, and show, access time;\n");
	ft_printf("%4s%-25s%s", "", "", "  with -l: show access time and sort by name;\n");
	ft_printf("%4s%-25s%s", "", "", "  otherwise: sort by access time, newest first\n");
	ft_printf("%4s%-25s%s", "-U", "", "do not sort; list entries in directory order\n");
	ft_printf("%4s%-25s%s", "-1", "", "list one file per line\n");
	ft_printf("      --help     display this help and exit\n");
	ft_printf("      --version  ouput version information and exit\n");
	return (-2);
}

int		check_opt(char av, unsigned long long *opt)
{
	if (av == 'a')
	{
		*opt &= ~OPT_ACAPS;
		*opt |= OPT_A;
	}
	else if (av == 'A')
	{
		*opt &= ~OPT_A;
		*opt |= OPT_ACAPS;
	}
	else if (av == 'R')
	{
		*opt |= OPT_RCAPS;
	}
	else if (av == 'r')
	{
		*opt |= OPT_R;
	}
	else if (av == 't')
	{
		*opt |= OPT_T;
		*opt |= OPT_SORT;
	}
	else if (av == 'l')
	{
		*opt &= ~OPT_CCAPS;
		*opt &= ~OPT_M;
		*opt |= OPT_L;
		*opt |= OPT_TOTAL;
	}
	else if (av == 'u')
	{
		*opt &= ~OPT_C;
		*opt |= OPT_U;
	}
	else if (av == 'f')
	{
		*opt &= ~OPT_SORT;
		*opt &= ~OPT_L;
		*opt &= ~OPT_GCAPS;
		*opt |= OPT_F;
		*opt |= OPT_UCAPS;
		*opt |= OPT_A;
	}
	else if (av == 'g')
	{
		*opt &= ~OPT_CCAPS;
		*opt &= ~OPT_M;
		*opt |= OPT_G;
		*opt |= OPT_TOTAL;
	}
	else if (av == 'G')
	{
		*opt |= OPT_GCAPS;
	}
	else if (av == 'd')
	{
		*opt |= OPT_D;
	}
	else if (av == 'M')
	{
		*opt |= OPT_MCAPS;
	}
	else if (av == '0')
	{
		*opt |= OPT_0;
	}
	else if (av == 'c')
	{
		*opt &= ~OPT_U;
		*opt |= OPT_C;
	}
	else if (av == 'C')
	{
		*opt &= ~OPT_M;
		*opt &= ~OPT_L;
		*opt |= OPT_CCAPS;
	}
	else if (av == 'U')
	{
		*opt &= ~OPT_SORT;
		*opt |= OPT_UCAPS;
	}
	else if (av == 'T')
	{
		*opt |= OPT_TCAPS;
	}
	else if (av == '1')
	{
		*opt &= ~OPT_M;
		*opt &= ~OPT_CCAPS;
		*opt |= OPT_1;
	}
	else if (av == 'A')
	{
		*opt |= OPT_ACAPS;
	}
	else if (av == 'h')
	{
		*opt &= ~OPT_SI;
		*opt |= OPT_H;
	}
	else if (av == 'm')
	{
		*opt &= ~OPT_L;
		*opt &= ~OPT_G;
		*opt &= ~OPT_N;
		*opt &= ~OPT_CCAPS;
		*opt |= OPT_M;
	}
	else if (av == 'n')
	{
		*opt &= ~OPT_CCAPS;
		*opt &= ~OPT_M;
		*opt |= OPT_N;
		*opt |= OPT_TOTAL;
	}
	else if (av == 'N')
	{
		*opt |= OPT_NCAPS;
	}
	else if (av == 'o')
	{
		*opt &= ~OPT_M;
		*opt |= OPT_O;
		*opt |= OPT_TOTAL;
	}
	else if (av == 'p')
	{
		*opt |= OPT_P;
	}
	else if (av == 'e')
	{
		*opt |= OPT_E;
	}
	else
	{
		custom_error("ft_ls: invalid option -- '%c'\n", av);
		custom_error("Try 'ft_ls --help' for more information.\n");
		return (-1);
	}
	return (0);
}

int		parse_option_line(char *av, unsigned long long *opt)
{
	if (ft_strbegin(av, "--"))
	{
		if (ft_strequ(av, "--help"))
			return (print_usage_stdin());
		else if (ft_strequ(av, "--version"))
		{
			ft_printf("ft_ls version 1.0\n");
			ft_printf("This program is free software; you may redistribute it\n");
			ft_printf("This program has absolutely no warranty ;)\n");
			ft_printf("\nWritten by Lucas Nicosia\n");
			return (-2);
		}
		else if (ft_strequ(av, "--dircolors"))
		{
			t_term_color			dir_colors[MAX_DIR_COLORS];
			t_extension_color	extension_colors[MAX_EXTENSION_COLORS];
			retrieve_ls_colors(dir_colors, extension_colors);
			print_ls_colors(dir_colors, extension_colors);
			return (-1);
		}
		else if (ft_strequ(av, "--extension-colors"))
		{
			t_term_color			dir_colors[MAX_DIR_COLORS];
			t_extension_color	extension_colors[MAX_EXTENSION_COLORS];
			retrieve_ls_colors(dir_colors, extension_colors);
			print_ls_extension_colors(extension_colors);
			return (-1);
		}
		else if (ft_strequ(av, "--filecolors"))
		{
			t_term_color			dir_colors[MAX_DIR_COLORS];
			t_extension_color	extension_colors[MAX_EXTENSION_COLORS];
			retrieve_ls_colors(dir_colors, extension_colors);
			print_ls_dir_colors(dir_colors);
			return (-1);
		}
		else if (ft_strequ(av, "--all"))
		{
			*opt &= ~OPT_ACAPS;
			*opt |= OPT_A;
		}
		else if (ft_strequ(av, "--almost-all"))
		{
			*opt &= ~OPT_A;
			*opt |= OPT_ACAPS;
		}
		else if (ft_strequ(av, "--author"))
		{
			*opt |= OPT_AUTHOR;
		}
		else if (ft_strequ(av, "--color"))
		{
			*opt |= OPT_GCAPS;
		}
		else if (ft_strequ(av, "--directory"))
		{
			*opt |= OPT_D;
		}
		else if (ft_strequ(av, "--human-readable"))
		{
			*opt &= ~OPT_SI;
			*opt |= OPT_H;
		}
		else if (ft_strequ(av, "--si"))
		{
			*opt &= ~OPT_H;
			*opt |= OPT_SI;
		}
		else if (ft_strequ(av, "--numeric-uid-gid"))
		{
			*opt |= OPT_N;
			*opt |= OPT_TOTAL;
		}
		else if (ft_strequ(av, "--literal"))
		{
			*opt |= OPT_NCAPS;
		}
		else if (ft_strequ(av, "--indicator-style=slash"))
		{
			*opt |= OPT_P;
		}
		else if (ft_strequ(av, "--reverse"))
		{
			*opt |= OPT_R;
		}
		else if (ft_strequ(av, "--recursive"))
		{
			*opt |= OPT_RCAPS;
		}
		else if (ft_strequ(av, "--extended"))
		{
			*opt |= OPT_E;
		}
		else if (ft_strequ(av, "--acl"))
		{
			*opt |= OPT_ACL;
		}
		else if (!ft_strequ(av, "--"))
		{
			custom_error("ft_ls: unrecognized option '%s'\n", av);
			custom_error("Try 'ft_ls --help' for more information.\n");
			return (-1);
		}
	}
	else
	{
		av++;
		while (*av)
		{
			if (check_opt(*av, opt))
				return (-1);
			av++;
		}
	}
	return (0);
}

/*
**	Checks if the given string is an option line (starting with '-')
*/

int				is_arg_an_option_line(char *av)
{
	return (ft_strlen(av) > 1 && av[0] == '-');
}

/*
**	Parse all the options by checking arguments starting with '-'
*/

int		parse_ls_options(int ac, char **av, unsigned long long *opt, int *real_args)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (is_arg_an_option_line(av[i]))
		{
			if (parse_option_line(av[i], opt))
				return (-1);
			(*real_args)--;
		}
		i++;
	}
	return (0);
}
