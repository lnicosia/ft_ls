/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 16:49:26 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/26 18:06:25 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"
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
	return ('x');
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
	
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		file = buf;
		if (lstat(buf, &stats))
			return (0);
	}
	return (1);
}

/*
**	Sets the right color according to the type of a file
*/

void	set_color(char *file, mode_t mode)
{
	ft_printf("{bold}");
	if (S_ISDIR(mode))
	{
		if (mode & 00777)
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

void	set_link_color(char *file)
{
	char	buf[256];
	ssize_t	size;
	t_stat	stats;
	
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		file = buf;
		if (lstat(buf, &stats))
		{
			ft_printf("{red}");
			return ;
		}
	}
	if (lstat(file, &stats))
	{
		ft_printf("{red}");
		return ;
	}
	//ft_printf("Setting color of link %s\n", buf);
	set_color(buf, stats.st_mode);
}

/*
**	Print the file name with colors.
**	Cuts the path after the last '/'
*/

void	print_file_name(t_stat file_stats, char *file, int opt)
{
	char	*name;
	char	buf[256];
	ssize_t	size;

	if (!(name = ft_strrchr(file, '/')))
		name = file;
	else
		name++;
	if (opt & OPT_GCAPS)
		set_color(file, file_stats.st_mode);
	ft_printf("%s", name);
	if (opt & OPT_GCAPS)
		ft_printf("{reset}");
	if (opt & OPT_L && S_ISLNK(file_stats.st_mode))
	{
		ft_printf(" -> ");
		if ((size = readlink(file, buf, 256)) == -1)
		{
			ft_perror("");
			return ;
		}
		buf[size] = '\0';
		if (opt & OPT_GCAPS)
			set_link_color(buf);
		ft_printf("%s", buf);
		if (opt & OPT_GCAPS)
			ft_printf("{reset}");
	}
}

/*
**	Print the detailed '-l' format of a file
*/

void	print_details(t_stat file_stats, char *file, int padding, int opt)
{
	t_passwd	*passwd;
	t_group		*group;
	char		time[30];

	(void)opt;
	print_type(file_stats.st_mode);
	print_permissions(file_stats.st_mode);
	ft_printf("%ld ", file_stats.st_nlink);
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
	ft_printf("%s ", passwd->pw_name);
	ft_printf("%s ", group->gr_name);
	print_size(file_stats.st_size, padding, opt);
	get_ls_time(time, ctime(&file_stats.st_mtime));
	ft_printf("%s ", time);
	print_file_name(file_stats, file, opt);
	ft_printf("\n");
}

/*
**	Print a file according to the options
*/

void	print_file(t_stat file_stats, char *file, int padding, int opt)
{
	if (opt & OPT_L)
		print_details(file_stats, file, padding, opt);
	else
		print_file_name(file_stats, file, opt);
}