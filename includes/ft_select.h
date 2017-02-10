#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <termios.h>
# include <curses.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include "../libft/libft.h"

# define CURP	((t_param*)g_env.current_param->data)

typedef struct		s_env
{
	int				max_len;
	int				nb_params;
	int				nb_selected;
	t_list			*params;
	t_list			*current_param;
	struct termios	old_term;
	int				screen_height;
	int				fd;
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

#endif
