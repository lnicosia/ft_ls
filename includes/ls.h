/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:18 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/23 17:41:08 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS_H
# define LS_H
# include "sys/types.h"
# include "libft.h"

typedef struct stat		t_stat;
typedef struct passwd	t_passwd;
typedef struct group	t_group;

int		ft_ls(int ac, char **av);
int		parse_option_line(char *av, int *opt);
int		analyze_args(char *file, int opt);
int		analyze_file(char *file, int opt);
void	print_file(t_stat file_stats, char *file, int opt);
void	get_ls_time(char *res, char *ctime);
void	print_size(off_t size);
int		sort_files(t_list **lst);

#endif