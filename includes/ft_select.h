#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <termios.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include "../libft/libft.h"

# define CURP	((t_param*)g_env.current_param->data)
# define STT	((t_param*)g_env.start->data)
# define END	((t_param*)g_env.end->data)

typedef struct		s_env
{
	int				max_len;
	int				nb_params;
	int				nb_selected;
	t_list			*params;
	t_list			*current_param;
	t_list			*start;
	t_list			*end;
	struct termios	old_term;
	int				screen_height;
	int				screen_width;
	int				fd;
	char				search;
	int				max_params;
	int				save_stdout;
}					t_env;

typedef struct	s_pos
{
	int			x;
	int			y;
}				t_pos;

typedef struct	s_param
{
	int			len;
	char		*str;
	t_pos		pos;
	int			col;
	char		selected;
}				t_param;

int				nmatch(char *s1, char *s2);
void				signals_set(void);

#endif
