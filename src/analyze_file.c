/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 15:11:07 by lnicosia          #+#    #+#             */
/*   Updated: 2022/05/24 15:37:08 by lnicosia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ls.h"
#include "directory.h"
#include "options.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <sys/xattr.h>
#include <sys/acl.h>
#ifdef ACL_PRESENT
# include <sys/acl.h>
#endif
#ifdef LIBACL_PRESENT
# include <acl/libacl.h>
#endif

/*
**	Analyze the directories contained in our sorted list
*/

void	analyze_list(t_dlist *lst, unsigned long long *opt)
{
	t_file	*file;

	while (lst && lst->prev)
		lst = lst->prev;
	while (lst)
	{
		file = (t_file*)lst->content;
		if (!S_ISDIR(file->stats.st_mode)
			|| ft_strequ(file->name + ft_strlen(file->name) - 2, "/.")
			|| ft_strequ(file->name + ft_strlen(file->name) - 3, "/.."))
		{
			lst = lst->next;
			continue;
		}
		analyze_directory(file->name, opt);
		lst = lst->next;
	}
}

/*
**	Analyse the current directory
**	prints all its files
**	then if '-R' was given, explore it
*/

int		analyze_directory(char *file_name, unsigned long long *opt)
{
	DIR 			*dir;
	struct dirent	*entry;
	char			*path;
	t_file			file;
	t_dlist			*dlst;
	t_dlist			*new;
	size_t			len;
	int				(*compare_func)(void *, void *);
	t_winsize		winsize;
	int				special_chars;
	size_t			filename_len;

	dlst = NULL;
	len = 0;
	compare_func = get_compare_func(*opt);
	if (!(dir = opendir(file_name)))
	{
		custom_error("ft_ls: cannot open directory '%s': ", file_name);
		*opt |= OPT_SMALL_ERROR;
		return (ft_perror(""));
	}
	if (*opt & OPT_RCAPS || *opt & OPT_NEWLINE || *opt & OPT_MULTIPLE_DIRS)
	{
		if (*opt & OPT_NEWLINE)
			ft_printf("\n");
		if (!(*opt & OPT_NEWLINE) && dir)
			*opt |= OPT_NEWLINE;
		special_chars = contains_special_chars(file_name);
		if (isatty(STDOUT_FILENO))
		{
			if (special_chars == 1)
			{
				if (*opt & OPT_NCAPS)
					ft_printf("%-s:\n", file_name);
				else
					ft_printf("'%-s':\n", file_name);
			}
			else if (special_chars == 2)
			{
				if (*opt & OPT_NCAPS)
					ft_printf("%-s:\n", file_name);
				else
					ft_printf("\"%-s\":\n", file_name);
			}
			else if (special_chars == 3)
			{
				if (*opt & OPT_NCAPS)
				{
					char* replaced = NULL;
					if (!(replaced = replace_char(file_name, '\n', "?")))
						ft_perror("replace_char:");
					ft_printf("%-s:\n", replaced);
					ft_strdel(&replaced);
				}
				else
				{
					char* replaced = NULL;
					if (!(replaced = replace_char(file_name, '\n', "'$'\\n''")))
						ft_perror("replace_char:");
					ft_printf("'%-s':\n", replaced);
					ft_strdel(&replaced);
				}
			}
			else
				ft_printf("%s:\n", file_name);
		}
		else
			ft_printf("%s:\n", file_name);
	}
	if (file_name[ft_strlen(file_name) - 1] == '/')
		file_name[ft_strlen(file_name) - 1] = '\0';
	while ((entry = readdir(dir)))
	{
		if (*opt & OPT_ACAPS
			&& (ft_strequ(entry->d_name, ".") || ft_strequ(entry->d_name, "..")))
			continue;
		else if (!(*opt & OPT_A || *opt & OPT_ACAPS) && entry->d_name[0] == '.')
			continue;
		ft_bzero(&file, sizeof(file));
		filename_len = ft_strlen(entry->d_name) + 2;
		if (isatty(STDOUT_FILENO))
		{
			special_chars = contains_special_chars(entry->d_name);
			if (special_chars == 1 || special_chars == 2)
			{
				if (!(*opt & OPT_NCAPS))
				{
					filename_len += 2;
					*opt |= OPT_SPECIAL_CHAR;
				}
			}
			else if (special_chars == 3)
			{
				if (!(*opt & OPT_NCAPS))
				{
					filename_len += 8;
					*opt |= OPT_SPECIAL_CHAR;
				}
				else
					filename_len += 1;
			}
		}
		len += filename_len;
		if (!(path = ft_strjoin(file_name, "/")))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			closedir(dir);
			return (ft_perror("ft_strjoin"));
		}
		if (!(path = ft_strjoin_free(path, entry->d_name)))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			closedir(dir);
			return (ft_perror("ft_strjoin_free"));
		}
		if (lstat(path, &file.stats))
		{
			custom_error("ft_ls: cannot access '%s': ", path);
			*opt |= OPT_SMALL_ERROR;
#ifdef _DIRENT_HAVE_D_TYPE
			if (DT_UNKNOWN & entry->d_type)
			{
				ft_dlstdelfront(&dlst, free_t_file);
				ft_strdel(&path);
				closedir(dir);
				return (ft_perror(""));
			}
			else
			{
				file.no_perm_but_print = 1;
				file.d_type = entry->d_type;
				ft_perror("");
			}
#else
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			closedir(dir);
			return (ft_perror(""));
#endif
		}
		char buf[256];
		if (S_ISLNK(file.stats.st_mode) && readlink(path, buf, 256) == -1)
		{
			custom_error("ft_ls: cannot read symbolic link '%s': ", path);
			*opt |= OPT_SMALL_ERROR;
			ft_perror("");
		}
		if (*opt & OPT_P && S_ISDIR(file.stats.st_mode))
		{
			filename_len++;
			len++;
		}
		file.namelen = filename_len - 2;
		file.name = path;
		if (listxattr(file.name, NULL, 0) > 0 && S_ISCHR(file.stats.st_mode))
		{
			file.has_extended = 1;
		}
#ifdef ACL_PRESENT
		if (!S_ISLNK(file.stats.st_mode))
		{
			acl_t	acl = acl_get_file(file.name, ACL_TYPE_ACCESS);
			if (acl != NULL)
			{
				acl_entry_t	entry;
				if (acl_get_entry(acl, ACL_FIRST_ENTRY, &entry) == 1)
				{
					acl_tag_t tag;
					//ft_printf("%s\n", file.name);
					//ft_printf("%s", acl_to_text(acl, NULL));
					while (acl_get_entry(acl, ACL_NEXT_ENTRY, &entry) == 1)
					{
						if (acl_get_tag_type(entry, &tag) == 0)
						{
							/*ft_printf("%-12s\n",	(tag == ACL_USER_OBJ) ?		"user_obj" :
												(tag == ACL_USER) ? 		"user" :
												(tag == ACL_GROUP_OBJ) ? 	"group_obj" :
												(tag == ACL_GROUP) ? 		"group" :
												(tag == ACL_MASK) ? 		"mask" :
												(tag == ACL_OTHER) ? 		"other" :
												"???");*/
							if (tag == ACL_MASK)
							{
								file.has_acl = 1;
							}
						}
					}
				}
				acl_free((void*)acl);
			}
		}
#else
		if (*opt & OPT_E)
			check_acl_with_popen(&file);
#endif
		if (!(new = ft_dlstnew(&file, sizeof(file))))
		{
			ft_dlstdelfront(&dlst, free_t_file);
			ft_strdel(&path);
			closedir(dir);
			return (ft_perror(""));
		}
		if (*opt & OPT_R && *opt & OPT_SORT)
			ft_dlstinsert_reverse(&dlst, new, compare_func);
		else
			ft_dlstinsert(&dlst, new, compare_func);
	}
	if (closedir(dir))
	{
		ft_dlstdelfront(&dlst, free_t_file);
		return (ft_perror(""));
	}
	if (!dlst)
	{
		if (*opt & OPT_TOTAL)
			ft_printf("total 0\n");
	}
	if (!isatty(STDOUT_FILENO))
		winsize.ws_col = 80;
	else
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	t_file* files;
	if (!(files = (t_file*)ft_dlist_to_array(dlst)))
		return (-1);
	if (*opt & OPT_CCAPS && dlst)
		print_dlist_col(files, ft_dlstlen(dlst), len, winsize.ws_col, *opt);
	else
		print_dlist(files, ft_dlstlen(dlst), winsize.ws_col, *opt);
	if (*opt & OPT_RCAPS)
		analyze_list(dlst, opt);
	ft_memdel((void**)&files);
	ft_dlstdelfront(&dlst, free_t_file);
	return (0);
}
