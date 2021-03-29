/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ls_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:54:31 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 12:25:56 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"

/*
**	Get the month [Jan,Feb,Mar...]
*/

void	get_month(char **buf, char **time)
{
	while (**time != ' ')
		(*time)++;
	(*time)++;
	while (**time != ' ')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
	**buf = ' ';
	(*buf)++;
}

/*
**	Get the day number
*/

void	get_day(char **buf, char **time)
{
	while (**time != ' ')
		(*time)++;
	(*time)++;
	while (**time != ' ')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
	**buf = ' ';
	(*buf)++;
	(*time)++;
}

/*
**	Get the hour and minutes
*/

void	get_hour(char **buf, char **time)
{
	while (**time != ':')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
	**buf = **time;
	(*buf)++;
	(*time)++;
	while (**time != ':')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
}

/*
**	Cut the date returned by time for the ls format
*/

void	get_ls_time(char *buf, t_stat stats, int opt)
{
	char	*time;

	if (opt & OPT_U)
		time = ctime(&stats.st_atime);
	else if (opt & OPT_C)
		time = ctime(&stats.st_mtime);
	else
		time = ctime(&stats.st_mtime);
	ft_bzero(buf, 30);
	get_month(&buf, &time);
	get_day(&buf, &time);
	get_hour(&buf, &time);
}