/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:22:18 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/10 17:48:13 by lnicosia         ###   ########.fr       */
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
typedef struct timespec	t_timespec;

typedef struct			s_file
{
	char				*name;
	size_t				namelen;
	t_stat				stats;
}						t_file;


int						ft_ls(int ac, char **av);
int						parse_ls_options(int ac, char **av, unsigned long long *opt,
	int *real_args);
int						parse_option_line(char *av, unsigned long long *opt);
	t_dlist					*analyze_args(int ac, char **av, unsigned long long *opt);
int						analyze_file(char *file, unsigned long long opt);
int						print_file(t_stat file_stats, char *file,
	t_ls_padding padding, unsigned long long opt);
int						print_file_name(t_stat file_stats, char *file,
	size_t padding, unsigned long long opt);
void					get_ls_time(char *res, t_stat stats, unsigned long long opt);
void					print_size(off_t size, int padding, unsigned long long opt);
void					print_size_short(off_t size);
int						sort_files(t_list **lst, int start, int end);
int						compare_none(void *s1, void *s2);
int						compare_names(void *s1, void *s2);
int						compare_names_no_case(void *s1, void *s2);
int						compare_times(void *s1, void *s2);
int						compare_atimes(void *s1, void *s2);
int						compare_ctimes(void *s1, void *s2);
void					print_dlist(t_file* files, size_t array_len,
	unsigned short winsize, unsigned long long opt);
void					print_dlist_reverse(t_file* files, size_t array_len,
	unsigned short winsize, unsigned long long opt);
void					free_t_file(void *file, size_t size);
int						analyze_directory(char *file_name, unsigned long long *opt);
int						(*get_compare_func(unsigned long long opt))(void*, void*);
int						print_dlist_col(t_file* files, size_t array_len, size_t len,
	unsigned short winsize, unsigned long long opt);
int						is_arg_an_option_line(char *av);
t_ls_padding			get_padding(t_file* files, size_t array_len, blksize_t *dir_size,
	unsigned long long opt);
int						get_doublelen(double size);

#endif
