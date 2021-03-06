/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 16:49:26 by lnicosia          #+#    #+#             */
/*   Updated: 2021/04/04 12:31:15 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
#include "ls_padding.h"
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

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
	ft_printf("%c%c%c%c%c%c%c%c%c ", get_permission('r', mode, S_IRUSR),
	get_permission('w', mode, S_IWUSR), get_permission('x', mode, S_IXUSR),
	get_permission('r', mode, S_IRGRP), get_permission('w', mode, S_IWGRP),
	get_permission('x', mode, S_IXGRP), get_permission('r', mode, S_IROTH),
	get_permission('w', mode, S_IWOTH), get_permission('x', mode, S_IXOTH));
}

/*
**	Verifies if a link is valid
*/

int		is_link_valid(char *file)
{
	char	buf[256];
	ssize_t	size;
	t_stat	stats;
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	
	if (!(last_slash = ft_strrchr(file, '/')))
	{
		if (!(dir = ft_strdup("./")))
		{
			ft_perror("ft_strdup");
			return (0);
		}
	}
	else
	{
		if (!(dir = ft_strsub(file, 0,
				ft_strlen(file) - ft_strlen(last_slash) + 1)))
		{
			ft_perror("ft_strsub");
			return (0);
		}
	}
	//ft_printf("Dir = %s\n", dir);
	dirlen = ft_strlen(dir);
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
		}
		//ft_printf("Link %s towards %s\n", file, buf);
		file = buf;
		if (stat(file, &stats))
		{
			//custom_error("stat %s: ", file);
			//ft_perror("");
			ft_strdel(&dir);
			return (0);
		}
	}
	ft_strdel(&dir);
	return (1);
}

/*
**	Sets the right color according to the type of a file
*/

void	set_color(char *file, mode_t mode, int opt)
{
	if (opt & OPT_B)
		ft_printf("{bold}");
	if (S_ISDIR(mode))
	{
		if ((mode & S_IWUSR) && (mode & S_IWGRP) && (mode & S_IWOTH))
			ft_printf("{reset}{bgreen}");
		ft_printf("{blue}");
	}
	else if (S_ISLNK(mode))
	{
		if (is_link_valid(file))
			ft_printf("{cyan}");
		else
			ft_printf("{red}");
	}
	else if (S_ISCHR(mode))
		ft_printf("{yellow}");	
	else if (S_ISBLK(mode))
		ft_printf("{reset}");
	else if (S_ISFIFO(mode))
		ft_printf("{reset}");
	else if (S_ISSOCK(mode))
		ft_printf("{reset}");
	else if (S_ISREG(mode))
	{
		if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH))
			ft_printf("{green}");
	}
}

/*
**	Sets the right color for a link
*/

void	print_link(char *file, int opt)
{
	char	buf[256];
	char	link[256];
	ssize_t	size;
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	t_stat stats;

	ft_printf(" -> ");
	ft_bzero(buf, 256);
	ft_bzero(link, 256);
	if (opt & OPT_B)
		ft_printf("{bold}");
	if ((size = readlink(file, link, 256)) == -1)
	{
		if (opt & OPT_GCAPS)
			ft_printf("{red}");
	}
	else
		link[size] = 0;
	if (!(last_slash = ft_strrchr(file, '/')))
	{
		if (!(dir = ft_strdup("./")))
		{
			ft_perror("ft_strdup");
			return ;
		}
	}
	else
	{
		if (!(dir = ft_strsub(file, 0,
				ft_strlen(file) - ft_strlen(last_slash) + 1)))
		{
			ft_perror("ft_strsub");
			return ;
		}
	}
	//ft_printf("Dir = %s\n", dir);
	dirlen = ft_strlen(dir);
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
		}
		//ft_printf("Link %s towards %s\n", file, buf);
		file = buf;
		if (stat(file, &stats))
		{
			//custom_error("stat %s: ", file);
			//ft_perror("");
			if (opt & OPT_GCAPS)
				ft_printf("{red}");
			break;
		}
	}
	if (stat(buf, &stats))
	{
		//custom_error("stat %s: ", file);
		//ft_perror("");
		if (opt & OPT_GCAPS)
			ft_printf("{red}");
	}
	if (opt & OPT_GCAPS)
		set_color(buf, stats.st_mode, opt);
	ft_printf("%s", link);
	ft_strdel(&dir);
}

/*
**	Print the file name with colors.
**	Cuts the path after the last '/'
*/

void	print_file_name(t_stat file_stats, char *file, size_t padding, int opt)
{
	char	*name;

	if (opt & OPT_GCAPS)
		set_color(file, file_stats.st_mode, opt);
	if (opt & OPT_ERROR)
		ft_printf(" ");
	if (opt & OPT_PATH || !(name = ft_strrchr(file, '/')))
		name = file;
	else
		name++;
	ft_printf("%-*s", padding, name);
	if (opt & OPT_GCAPS)
		ft_printf("{reset}");
	if (opt & OPT_L && S_ISLNK(file_stats.st_mode))
	{
		print_link(file, opt);	
		if (opt & OPT_GCAPS)
			ft_printf("{reset}");
	}
}

/*
**	Print the detailed '-l' format of a file
*/

void	print_details(t_stat file_stats, char *file, t_ls_padding padding,
int opt)
{
	t_passwd	*passwd;
	t_group		*group;
	char		time[30];

	print_type(file_stats.st_mode);
	print_permissions(file_stats.st_mode);
	ft_printf("%*ld ", padding.links, file_stats.st_nlink);
	if (!(passwd = getpwuid(file_stats.st_uid)))
	{
		ft_perror("Error: ");
		return ;
	}
	if (!(group = getgrgid(file_stats.st_gid)))
	{
		ft_perror("Error: ");
		return ;
	}
	ft_printf("%-*s ", padding.user, passwd->pw_name);
	ft_printf("%-*s ", padding.group, group->gr_name);
	print_size(file_stats.st_size, padding.size, opt);
	get_ls_time(time, file_stats, opt);
	ft_printf("%s", time);
	if (opt & OPT_TCAPS)
	{
		ft_printf(":%.2ld.%ld", file_stats.st_atim.tv_sec % 60,
		file_stats.st_atim.tv_nsec);
	}
	ft_printf(" ");
	print_file_name(file_stats, file, 0, opt);
	ft_printf("\n");
}

/*
**	Print a file according to the options
*/

void	print_file(t_stat file_stats, char *file, t_ls_padding padding,
int opt)
{
	if (opt & OPT_L)
		print_details(file_stats, file, padding, opt);
	else
	{
		print_file_name(file_stats, file, 0, opt);
		if (!isatty(STDOUT_FILENO))
			ft_printf("\n");
	}
}