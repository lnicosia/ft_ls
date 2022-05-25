/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ls_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:54:31 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/09 17:10:38 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "options.h"

/*
**	Get the year [2021, 2022...]
*/

void	get_year(char **buf, char **time)
{
	while (**time != ' ')
	{
		(*time)++;
	}
	while (**time != '\n')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
}

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
}

/*
**	Get the day number
*/

void	get_day(char **buf, char **time)
{
	while (**time == ' ')
	{
		**buf = **time;
		(*buf)++;
		(*time)++;
	}
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

void	get_ls_time(char *buf, t_stat stats, unsigned long long opt)
{
	char	*time_str;
	time_t	used_time;
	time_t	current_time;

	if (opt & OPT_U)
		used_time = stats.st_atime;
	else if (opt & OPT_C)
		used_time = stats.st_ctime;
	else
		used_time = stats.st_mtime;
	ft_bzero(buf, 30);
	time_str = ctime(&used_time);
	current_time = time(NULL);
	get_month(&buf, &time_str);
	get_day(&buf, &time_str);
	if (current_time - used_time >= 15778458 || current_time < used_time)
	{
		get_year(&buf, &time_str);
	}
	else
	{
		get_hour(&buf, &time_str);
	}
}
