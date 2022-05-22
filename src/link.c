/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 18:16:27 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/20 16:47:13 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"
#include "options.h"

int		should_print_link(char *file)
{
	char	buf[256];
	ssize_t	size;
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	int		valid;

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
	dirlen = ft_strlen(dir);
	valid = 0;
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			buf[(size_t)size + dirlen] = '\0';
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
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	int		valid;

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
	dirlen = ft_strlen(dir);
	valid = 0;
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			buf[(size_t)size + dirlen] = '\0';
		}
		file = buf;
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
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	int		valid;

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
	dirlen = ft_strlen(dir);
	valid = 0;
	while ((size = readlink(file, buf, 256)) != -1)
	{
		valid = 1;
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			buf[(size_t)size + dirlen] = '\0';
		}
		file = buf;
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
	char	*last_slash;
	char	*dir;
	size_t	dirlen;
	t_stat stats;

	ft_printf(" -> ");
	ft_bzero(buf, 256);
	ft_bzero(link, 256);
	if ((size = readlink(file, link, 256)) == -1)
	{
		//if (opt & OPT_GCAPS)
		//	ft_printf("{red}");
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
	dirlen = ft_strlen(dir);
	while ((size = readlink(file, buf, 256)) != -1)
	{
		buf[size] = '\0';
		if (buf[0] != '/')
		{
			ft_memmove(buf + dirlen, buf, (size_t)size);
			ft_memmove(buf, dir, dirlen);
			buf[(size_t)size + dirlen] = '\0';
		}
		file = buf;
		if (stat(file, &stats))
		{
			//if (opt & OPT_GCAPS)
			//	ft_printf("{red}");
			break;
		}
	}
	if (stat(buf, &stats))
	{
		custom_error("%s error:\n", buf);
		ft_perror("");
		//if (opt & OPT_GCAPS)
		//	ft_printf("{red}");
	}
	if (opt & OPT_GCAPS)
		set_color(buf, stats.st_mode, stats);
	ft_printf("%s", link);
	ft_strdel(&dir);
}
