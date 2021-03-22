/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:18 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/22 17:12:18 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS_H
# define LS_H

typedef struct stat	t_stat;

int		ft_ls(int ac, char **av);
int		parse_option_line(char *av, int *opt);
int		analyze_file(char *file, int opt);
void	print_file(t_stat file_stats, char *file, int opt);

#endif