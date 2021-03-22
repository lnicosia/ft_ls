/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:35:33 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 10:48:15 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Struct data registering all the attributes of a directory
*/

#ifndef DIRECTORY_H
# define DIRECTORY_H

# include <sys/stat.h>

typedef struct stat t_stat;

typedef struct	s_directory
{
	const char	*name;
	t_stat		stats;
}				t_directory;


#endif