/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 16:49:26 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/24 14:34:15 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include "ls_padding.h"
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#ifdef ACL_PRESENT
# include <sys/acl.h>
# include <acl/libacl.h>
#endif

/*
**	Print the type of the file as the first letter [-dbclps]
*/

void	print_type(mode_t mode)
{
	if (S_ISDIR(mode))
		ft_printf("d");
	else if (S_ISLNK(mode))
		ft_printf("l");
	else if (S_ISCHR(mode))
		ft_printf("c");
	else if (S_ISBLK(mode))
		ft_printf("b");
	else if (S_ISFIFO(mode))
		ft_printf("p");
	else if (S_ISSOCK(mode))
		ft_printf("s");
	else
		ft_printf("-");
}

/*
**	Returns the character to print according to the permission
**	we want to check: [r,w,x] if granted or '-' if not
*/

char	get_permission(char c, mode_t mode, unsigned int permission)
{

	if (mode & permission)
		return (c);
	return ('-');
}

/*
**	Print permissions of a file under the 'drwxrwxrwx' format
*/

void	print_permissions(mode_t mode)
{
	ft_printf("%c%c", get_permission('r', mode, S_IRUSR),
	get_permission('w', mode, S_IWUSR));

	if (mode & S_ISUID)
	{
		if (mode & S_IXUSR)
			ft_printf("s");
		else
			ft_printf("S");
	}
	else
		ft_printf("%c", get_permission('x', mode, S_IXUSR));

	ft_printf("%c%c",
		get_permission('r', mode, S_IRGRP), get_permission('w', mode, S_IWGRP));

	if (mode & S_ISGID)
	{
		if (mode & S_IXGRP)
			ft_printf("s");
		else
			ft_printf("S");
	}
	else
		ft_printf("%c", get_permission('x', mode, S_IXGRP));

	ft_printf("%c%c",
		get_permission('r', mode, S_IROTH), get_permission('w', mode, S_IWOTH));
	if (mode & S_ISVTX && S_ISDIR(mode))
	{
		if (!(mode & S_IXOTH))
			ft_printf("T");
		else
			ft_printf("t");
	}
	else
		ft_printf("%c", get_permission('x', mode, S_IXOTH));
}

/*
**	Print the file name with colors.
**	Cuts the path after the last '/'
*/

int		print_file_name(t_stat file_stats, char *file, size_t padding, unsigned long long opt)
{
	char	*name;
	int		len;
	int		special_chars;

	len = 0;
	if (opt & OPT_GCAPS)
		set_color(file, file_stats.st_mode, file_stats, 0);
	if (opt & OPT_ERROR)
		len += ft_printf(" ");
	if (opt & OPT_PATH || !(name = ft_strrchr(file, '/')))
		name = file;
	else
		name++;
	special_chars = contains_special_chars(name);
	if (special_chars != 0)
	{
		if (isatty(STDOUT_FILENO))
		{
			if (special_chars == 1)
			{
				if (opt & OPT_NCAPS)
					len += ft_printf("%-s", name);
				else
					len += ft_printf("'%-s'", name);
			}
			else if (special_chars == 2)
			{
				if (opt & OPT_NCAPS)
					len += ft_printf("%-s", name);
				else
					len += ft_printf("\"%-s\"", name);
			}
			else if (special_chars == 3)
			{
				if (opt & OPT_NCAPS)
				{
					char* replaced = NULL;
					if (!(replaced = replace_char(name, '\n', "?")))
						ft_perror("replace_char:");
					len += ft_printf("%-s", replaced);
					ft_strdel(&replaced);
				}
				else
				{
					char* replaced = NULL;
					if (!(replaced = replace_char(name, '\n', "'$'\\n''")))
						ft_perror("replace_char:");
					len += ft_printf("'%-s'", replaced);
					ft_strdel(&replaced);
				}
			}
		}
		else
			len += ft_printf("%-s", name);
	}
	else
	{
		if (opt & OPT_SPECIAL_CHAR && !(opt & OPT_NCAPS)
			&& (opt & OPT_CCAPS ||
				opt & OPT_L || opt & OPT_G || opt & OPT_N || opt & OPT_O))
			len += ft_printf("{reset} ");
		if (opt & OPT_GCAPS)
			set_color(file, file_stats.st_mode, file_stats, 0);
		len += ft_printf("%-s", name);
	}
	if (opt & OPT_GCAPS)
		ft_printf("{reset}");
	if (S_ISDIR(file_stats.st_mode) && opt & OPT_P)
	{
		len += ft_printf("/");
		padding--;
	}
	if (opt & OPT_CCAPS && padding != 0)
	{
		if (special_chars == 1 || special_chars == 2)
		{
			if (!(opt & OPT_NCAPS))
				padding -= 1;
		}
		else if (special_chars == 3)
		{
			if (!(opt & OPT_NCAPS))
			{
				//if (opt & OPT_P)
				//	padding -= 6;
				//else
					padding -= 7;
			}
		}
		if (opt & OPT_SPECIAL_CHAR) //|| opt & OPT_P)
			padding--;
		ft_printf("%*c", padding - ft_strlen(name), ' ');
	}
	char buf[256];
	if ((opt & OPT_L || opt & OPT_G || opt & OPT_N || opt & OPT_O)
		&& S_ISLNK(file_stats.st_mode) && readlink(file, buf, 256) != -1)
	{
		print_link(file, opt);
		if (opt & OPT_GCAPS)
			ft_printf("{reset}");
	}
	return (len);
}

/*
**	Print the device ids for character and block files
*/

void	print_device_id(t_stat file_stats, t_ls_padding padding)
{
	ft_printf("%*ld, %*ld ",
		padding.major_size, major(file_stats.st_rdev),
		padding.minor_size, minor(file_stats.st_rdev));
}

/*
**	Print the detailed '-l' format of a file
*/

void	print_details(t_file file, t_ls_padding padding,
unsigned long long opt)
{
	t_passwd	*passwd;
	t_group		*group;
	char		time[30];

	print_type(file.stats.st_mode);
	print_permissions(file.stats.st_mode);
	if (file.has_acl || file.has_extended)
		ft_printf("+");
	else if (padding.xattr)
		ft_printf(" ");
	ft_printf(" ");
	ft_printf("%*ld ", padding.links, file.stats.st_nlink);
	if (!(passwd = getpwuid(file.stats.st_uid)))
	{
		opt |= OPT_N;
	}
	if (!(group = getgrgid(file.stats.st_gid)))
	{
		opt |= OPT_N;
	}
	if (!(opt & OPT_G))
	{
		if (opt & OPT_N)
			ft_printf("%*d ", padding.user, file.stats.st_uid);
		else
			ft_printf("%-*s ", padding.user, passwd->pw_name);
	}
	if (!(opt & OPT_O))
	{
		if (opt & OPT_N)
			ft_printf("%*d ", padding.group, file.stats.st_gid);
		else
			ft_printf("%-*s ", padding.group, group->gr_name);
	}
	if (S_ISCHR(file.stats.st_mode) || S_ISBLK(file.stats.st_mode))
		print_device_id(file.stats, padding);
	else
		print_size(file.stats.st_size, padding.size, opt);
	get_ls_time(time, file.stats, opt);
	ft_printf("%s", time);
	if (opt & OPT_TCAPS)
	{
		ft_printf(":%.2ld.%ld", file.stats.st_atim.tv_sec % 60,
		file.stats.st_atim.tv_nsec);
	}
	ft_printf(" ");
	print_file_name(file.stats, file.name, 0, opt);
#ifdef ACL_PRESENT
	if (!(opt & OPT_E))
		return ;
	acl_t acl = acl_get_file(file.name, ACL_TYPE_ACCESS);
	if (acl != (acl_t)NULL)
	{
		char* str = acl_to_text(acl, NULL);
		if (str)
			ft_printf("\n%s", str);
		acl_free((void*)str);
		acl_free((void*)acl);
	}
#endif
}

/*
**	Print a file according to the options
*/

int		print_file(t_file file, t_ls_padding padding,
unsigned long long opt)
{
	if (opt & OPT_L || opt & OPT_G || opt & OPT_N || opt & OPT_O)
		print_details(file, padding, opt);
	else
	{
		return (print_file_name(file.stats, file.name, 0, opt));
	}
	return (0);
}
