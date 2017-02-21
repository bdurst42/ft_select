/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 17:18:59 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/20 17:19:02 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <termios.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <term.h>

# define CURP	((t_param*)g_e.cur_p->data)
# define CM		tgetstr("cm", NULL)

typedef struct		s_env
{
	int				max_len;
	int				nb_p;
	int				nb_selected;
	t_list			*params;
	t_list			*cur_p;
	t_list			*start;
	t_list			*end;
	struct termios	old_term;
	int				s_height;
	int				s_width;
	int				fd;
	char			search;
	int				max_p;
	int				save_stdout;
}					t_env;

typedef struct		s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef struct		s_param
{
	int				len;
	char			*str;
	t_pos			pos;
	int				col;
	char			selected;
}					t_param;

extern t_env		g_e;

void				delete_param(void);

void				print_list(void);
t_list				*get_params(int ac, char **av);

void				right(char *res);

void				left(char *res);

void				up(char *res);
void				down(char *res, char arrow);

void				search(char *str);

void				deselect_all(void);
void				select_all(void);
void				free_strjoin(char *s1, char **s2, char c);

void				underline(void);
void				reverse_video(char *str);
void				reset_term(char out);
char				term_clear(void);
int					my_outc(int c);

void				get_new_window_size(void);
void				update_window(void);

int					read_stdin(void);

int					nmatch(char *s1, char *s2);

void				signals_set(void);

void				init_term(struct termios *term);

#endif
