/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ls_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:54:31 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/23 15:03:53 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Get the month [Jan,Feb,Mar...]
*/

void	get_month(char **buf, char **ctime)
{
	while (**ctime != ' ')
		(*ctime)++;
	(*ctime)++;
	while (**ctime != ' ')
	{
		**buf = **ctime;
		(*buf)++;
		(*ctime)++;
	}
	**buf = ' ';
	(*buf)++;
}

/*
**	Get the day number
*/

void	get_day(char **buf, char **ctime)
{
	while (**ctime != ' ')
		(*ctime)++;
	(*ctime)++;
	while (**ctime != ' ')
	{
		**buf = **ctime;
		(*buf)++;
		(*ctime)++;
	}
	**buf = ' ';
	(*buf)++;
	(*ctime)++;
}

/*
**	Get the hour and minutes
*/

void	get_hour(char **buf, char **ctime)
{
	while (**ctime != ':')
	{
		**buf = **ctime;
		(*buf)++;
		(*ctime)++;
	}
	**buf = **ctime;
	(*buf)++;
	(*ctime)++;
	while (**ctime != ':')
	{
		**buf = **ctime;
		(*buf)++;
		(*ctime)++;
	}
}

/*
**	Cut the date returned by ctime for the ls format
*/

void	get_ls_time(char *buf, char *ctime)
{
	ft_bzero(buf, 30);
	get_month(&buf, &ctime);
	get_day(&buf, &ctime);
	get_hour(&buf, &ctime);
}