/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:18 by lnicosia          #+#    #+#             */
/*   Updated: 2021/03/29 12:23:00 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS_H
# define LS_H
# include "sys/types.h"
# include "sys/stat.h"
# include "libft.h"
# include "ls_padding.h"

typedef struct stat		t_stat;
typedef struct passwd	t_passwd;
typedef struct group	t_group;
typedef struct winsize	t_winsize;

typedef struct			s_file
{
	char				*name;
	t_stat				stats;
}						t_file;


int						ft_ls(int ac, char **av);
int						parse_option_line(char *av, int *opt);
int						analyze_args(char *file, int new_line, int opt);
int						analyze_file(char *file, int new_line, int opt);
void					print_file(t_stat file_stats, char *file,
t_ls_padding padding, int opt);
void					get_ls_time(char *res, t_stat stats, int opt);
void					print_size(off_t size, int padding, int opt);
void					print_size_short(off_t size);
int						sort_files(t_list **lst, int start, int end);
int						compare_none(void *s1, void *s2);
int						compare_names(void *s1, void *s2);
int						compare_names_no_case(void *s1, void *s2);
int						compare_times(void *s1, void *s2);
int						compare_atimes(void *s1, void *s2);
int						compare_ctimes(void *s1, void *s2);
void					print_dlist(t_dlist *dlst, int opt);
void					print_dlist_reverse(t_dlist *dlst, int opt);
void					free_t_file(void *file, size_t size);
int						analyze_directory(char *file_name, int new_line,
int opt);
int						(*get_compare_func(int opt))(void*, void*);

#endif