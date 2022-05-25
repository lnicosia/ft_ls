/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 18:16:27 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/24 15:10:00 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "options.h"
#include <sys/param.h>

char*	get_dir(char* file)
{
	char*	last_slash;
	char*	dir;

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
	return dir;
}

int		should_print_link(char *file)
{
	char	buf[256];
	ssize_t	size;
	char	*dir;
	size_t	dirlen;
	int		valid;
	t_stat	stats;

	dir = get_dir(file);
	if (dir == NULL)
		return (0);
	dirlen = ft_strlen(dir);
	valid = 0;
	if (stat(file, &stats))
	{
		return (0);
	}
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			ft_bzero(buf + size + dirlen, 256 - (size_t)size - dirlen);
		}
		file = buf;
	}
	ft_strdel(&dir);
	return (valid);
}

/*
**	Verifies if a link is valid
*/

int		is_link_valid(char *file)
{
	char	buf[256];
	ssize_t	size;
	t_stat	stats;
	char	*dir;
	size_t	dirlen;
	int		valid;

	dir = get_dir(file);
	if (dir == NULL)
		return (0);
	dirlen = ft_strlen(dir);
	valid = 0;
	if (stat(file, &stats))
	{
		return (0);
	}
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			ft_bzero(buf + size + dirlen, 256 - (size_t)size - dirlen);
		}
		file = buf;
		ft_strdel(&dir);
		if (!(dir = get_dir(file)))
			return (0);
		dirlen = ft_strlen(dir);
		if (stat(file, &stats))
		{
			ft_strdel(&dir);
			return (0);
		}
	}
	ft_strdel(&dir);
	return (valid);
}

/*
**	Get the last link
*/

char*	get_link(char *file)
{
	char	buf[256];
	ssize_t	size;
	t_stat	stats;
	char	*dir;
	size_t	dirlen;
	int		valid;

	dir = get_dir(file);
	if (dir == NULL)
		return (0);
	dirlen = ft_strlen(dir);
	valid = 0;
	if (stat(file, &stats))
	{
		custom_error("ft_ls: cannot access '%s': ", file);
		ft_perror("");
		return (0);
	}
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			ft_bzero(buf + size + dirlen, 256 - (size_t)size - dirlen);
		}
		file = buf;
		ft_strdel(&dir);
		if (!(dir = get_dir(file)))
			return (0);
		dirlen = ft_strlen(dir);
		if (stat(file, &stats))
		{
			ft_strdel(&dir);
			return (0);
		}
	}
	ft_strdel(&dir);
	if (valid == 1)
		return (ft_strdup(buf));
	return (NULL);
}

/*
**	Sets the right color for a link
*/

void	print_link(char *file, unsigned long long opt)
{
	char	buf[256];
	char	link[256];
	ssize_t	size;
	char	*dir;
	size_t	dirlen;
	t_stat	stats;

	ft_printf(" -> ");
	ft_bzero(buf, 256);
	ft_bzero(link, 256);
	if ((size = readlink(file, link, 256)) == -1)
	{
	}
	else
		link[size] = 0;
	dir = get_dir(file);
	if (dir == NULL)
		return ;
	dirlen = ft_strlen(dir);
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			ft_bzero(buf + size + dirlen, 256 - (size_t)size - dirlen);
		}
		file = buf;
		ft_strdel(&dir);
		if (!(dir = get_dir(file)))
			return ;
		dirlen = ft_strlen(dir);
		if (stat(file, &stats))
		{
			break;
		}
	}
	//	If this lstat fails it is an orphan link
	if (lstat(buf, &stats))
	{
		if (opt & OPT_GCAPS)
			set_color(buf, stats.st_mode, stats, 1);
	}
	else if (opt & OPT_GCAPS)
		set_color(buf, stats.st_mode, stats, 0);
	ft_printf("%s", link);
	ft_strdel(&dir);
}
