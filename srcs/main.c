/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 23:29:23 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/10 17:24:48 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <term.h>

static t_env	g_env;

static void		init_term(struct termios *term)
{
	char           *name_term;

	if ((name_term = getenv("TERM")) == NULL)
	{
		ft_putendl("getenv failure");
		exit(-1);
	}
	if (tgetent(NULL, name_term) == ERR)
	{
		ft_putendl("tgetent failure");
		exit(-1);
	}
	if (tcgetattr(0, term) == -1)
	{
		ft_putendl("tcgetattr failure");
		exit(-1);
	}
}

static int		my_outc(int c)
{
	ft_fputchar(c, g_env.fd);
	return (0);
}

static char		term_clear()
{
	char *res;

	if ((res = tgetstr("cl", NULL)) == NULL)
		return (0);
	tputs(res, 0, &my_outc);
	return (1);
}

static void		reset_term(char out)
{
	term_clear();
	if (tcsetattr(0, TCSADRAIN, &g_env.old_term) == -1)
	{
		ft_putendl("tcsetattr failure");
		close(g_env.fd);
		exit(-1);
	}
	if (out)
	{
		exit(-1);
		close(g_env.fd);
	}
}

static t_list	*get_params(int ac, char **av)
{
	int		i;
	t_list	*params;
	t_param *param;
	int		len;
	int		max_params;

	params = NULL;
	i = -1;
	while (++i < ac)
	{
		len = ft_strlen(av[i + 1]);
		if (len > g_env.max_len)
			g_env.max_len = len;
	}
	max_params = g_env.screen_height * (g_env.screen_width / (g_env.max_len + 1));
	i = -1;
	while (++i < ac)
	{
		if (!(param = (t_param*)malloc(sizeof(t_param))))
			reset_term(1);
		param->selected = 0;
		param->len = ft_strlen(av[i + 1]);
		param->str = av[i + 1];
		if (i <= max_params)
		{
			param->col = i / g_env.screen_height;
			param->pos.y = i % g_env.screen_height;
			param->pos.x = param->col * (g_env.max_len + 1);
		}
		else
			param->col = -1;
		if (i == max_params)
			g_env.end = params->prev;
		ft_c_node_push_back(&params, param);
	}
	g_env.start = params;
	if (!g_env.end)
		g_env.end = params->prev;
	return (params);
}

static void		get_param_next_at(t_list *elem, int nb_i)
{
	int	i;

	i = -1;
	while (++i < nb_i)
		elem = elem->next;
	g_env.current_param = elem;
}

static void		get_param_prev_at(t_list *elem, int nb_i)
{
	while (nb_i--)
		elem = elem->prev;
	g_env.current_param = elem;
}

static void		get_param_by_end(t_list *elem, int y)
{
	t_param	*param;

	param = elem->data;
	while (param->pos.y > y)
	{
		elem = elem->prev;
		param = elem->data;
	}
	g_env.current_param = elem;
}

static void		reverse_video(char *str)
{
	tputs(tgetstr("mr", NULL), 1, &my_outc);
	ft_fputstr(str, g_env.fd);
	tputs(tgetstr("me", NULL), 1, &my_outc);
}

static void		up(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_env.fd);
	g_env.current_param = g_env.current_param->prev;
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		down(char *res, char arrow)
{
	if (arrow)
	{
		if (CURP->selected)
			reverse_video(CURP->str);
		else
			ft_fputstr(CURP->str, g_env.fd);
	}
	g_env.current_param = g_env.current_param->next;
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		left(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_env.fd);
	if (CURP->col * g_env.screen_height + CURP->pos.y + 1 - g_env.screen_height >= 0)
		get_param_prev_at(g_env.current_param, g_env.screen_height);
	else if (g_env.nb_params > g_env.screen_height)
		get_param_by_end(g_env.params->prev, CURP->pos.y);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		right(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_env.fd);
	if ((CURP->col + 1) * g_env.screen_height + CURP->pos.y + 1 <= g_env.nb_params)
		get_param_next_at(g_env.current_param, g_env.screen_height);
	else if ((CURP->col + 1) * g_env.screen_height < g_env.nb_params)
		g_env.current_param = g_env.params->prev;
	else if (g_env.nb_params > g_env.screen_height)
		get_param_next_at(g_env.params, CURP->pos.y);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		underline(void)
{
	tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
	if (CURP->selected)
		tputs(tgetstr("mr", NULL), 1, &my_outc);
	tputs(tgetstr("us", NULL), 1, &my_outc);
	ft_fputstr(CURP->str, g_env.fd);
	tputs(tgetstr("ue", NULL), 1, &my_outc);
	if (CURP->selected)
		tputs(tgetstr("me", NULL), 1, &my_outc);
	tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		print_list(char first)
{
	t_list	*tmp;
	t_param	*param;
	char	*res;
	int		i;

	res = tgetstr("cm", NULL);
	i = -1;
	tmp = g_env.start;
	while (tmp != g_env.end->next || i == -1)
	{
		param = tmp->data;
		param->col = ++i / g_env.screen_height;
		param->pos.y = i % g_env.screen_height;
		param->pos.x = param->col * (g_env.max_len + 1);
		tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
		if (param->selected)
			reverse_video(param->str);
		else
			ft_fputstr(param->str, g_env.fd);
		tmp = tmp->next;
	}
	if (first)
		tputs(tgoto(res, 0, 0), 1, &my_outc);
	else
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		compute_max_len(void)
{
	t_list	*tmp;
	t_param	*param;

	tmp = g_env.params;
	param = tmp->data;
	g_env.max_len = 0;
	if (param->len > g_env.max_len)
		g_env.max_len = param->len;
	tmp = tmp->next;
	while (tmp != g_env.params)
	{
		param = tmp->data;
		if (param->len > g_env.max_len)
			g_env.max_len = param->len;
		tmp = tmp->next;
	}
}

static void		free_param(t_param *param)
{
	if (param)
		free(param);
}

static void		move_pos(void)
{
	t_list	*tmp;
	t_param	*param;
	t_param	*prev_param;

	tmp = g_env.params->prev;
	while (tmp != g_env.current_param)
	{
		param = tmp->data;
		prev_param = tmp->prev->data;
		param->pos.x = prev_param->pos.x;
		param->pos.y = prev_param->pos.y;
		tmp = tmp->prev;
	}
}

static void		delete_param(void)
{
	t_list	*prev;

	if (CURP->len == g_env.max_len)
		compute_max_len();
	move_pos();
	prev = g_env.current_param->prev;
	g_env.current_param = g_env.current_param->next;
	if (g_env.current_param->prev == g_env.params)
		g_env.params = g_env.params->next;
	ft_pop_node(g_env.current_param->prev, (void (*)(void *))&free_param);
	g_env.current_param->prev = prev;
	--g_env.nb_params;
	term_clear();
	if (!g_env.nb_params)
		reset_term(1);
	print_list(0);
}

static void		clear_last_searchs(void)
{
	t_list	*tmp;
	t_param	*param;

		param = NULL;
		tmp = g_env.start;
		while (tmp != g_env.end->next || !param)
		{
			param = tmp->data;
		if (param->selected == 2)
		{
			tputs(tgoto(tgetstr("cm", NULL), param->pos.x, param->pos.y), 1, &my_outc);
			ft_fputstr(param->str, g_env.fd);
				param->selected = 0;
				--g_env.nb_selected;
		}
		tmp = tmp->next;
		}
		tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		search(char *str)
{
	t_list	*tmp;
	t_param	*param;
	int		len;

	clear_last_searchs();
	param = NULL;
	if ((len = ft_strlen(str)) > g_env.max_len)
			  return ;
	tmp = g_env.start;
	while (tmp != g_env.end->next || !param)
	{
			  param = tmp->data;
			  if (!ft_strncmp(str, param->str, len) || (ft_strchr(str, '*') && nmatch(param->str, str)))
			  {
						 if (CURP->selected)
									reverse_video(CURP->str);
						 else
									ft_fputstr(CURP->str, g_env.fd);
						 g_env.current_param = tmp;
						 tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
						 reverse_video(param->str);
						 tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
						 param->selected = 2;
						 ++g_env.nb_selected;
			  }
			  tmp = tmp->next;
	}
	underline();
}

static void		deselect_all(void)
{
		  t_list	*tmp;
		  t_param	*param;

		  tmp = g_env.params;
		  param = tmp->data;
		  if (param->selected)
		  {
					 tputs(tgoto(tgetstr("cm", NULL), param->pos.x, param->pos.y), 1, &my_outc);
					 ft_fputstr(param->str, g_env.fd);
					 --g_env.nb_selected;
					 param->selected = 0;
		  }
		  tmp = tmp->next;
		  while (tmp != g_env.params)
		  {
					 param = tmp->data;
					 if (param->selected)
					 {
								tputs(tgoto(tgetstr("cm", NULL), param->pos.x, param->pos.y), 1, &my_outc);
								ft_fputstr(param->str, g_env.fd);
						 --g_env.nb_selected;
						 param->selected = 0;
			  }
			  tmp = tmp->next;
	}
	tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
	underline();
}

static void		select_all(void)
{
	t_list	*tmp;
	t_param	*param;

	tmp = g_env.params;
	param = tmp->data;
	if (!param->selected)
	{
		tputs(tgoto(tgetstr("cm", NULL), param->pos.x, param->pos.y), 1, &my_outc);
		reverse_video(param->str);
		++g_env.nb_selected;
		param->selected = 1;
	}
	tmp = tmp->next;
	while (tmp != g_env.params)
	{
			  param = tmp->data;
			  if (!param->selected)
			  {
						 tputs(tgoto(tgetstr("cm", NULL), param->pos.x, param->pos.y), 1, &my_outc);
						 reverse_video(param->str);
						 ++g_env.nb_selected;
						 param->selected = 1;
			  }
			  tmp = tmp->next;
	}
	tputs(tgoto(tgetstr("cm", NULL), CURP->pos.x, CURP->pos.y), 1, &my_outc);
	underline();
}
static void	free_strjoin(char *s1, char **s2, char c)
{
	char	*tmp;

	tmp = *s2;
  	if (c)
		*s2 = ft_strjoin(s1, *s2);
	else
		*s2 = ft_strjoin(*s2, s1);
	if (tmp)
		free(tmp);
}

static int		get_stdin(void)
{
	int				ret;
	char			*res;
	char			buff[4];
	char			*srch;

	res = tgetstr("cm", NULL);
	underline();
	srch = NULL;
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		if (buff[0] == 27)
		{
			if (buff[1] == 0)
			{
				if (g_env.search)
				{
					g_env.search = 0;
					srch = NULL;
				}
				else
					return (0);
			}
			else if (buff[1] == '[' && buff[2] == 'A')
				up(res);
			else if (buff[1] == '[' && buff[2] == 'B')
				down(res, 1);
			else if (buff[1] == '[' && buff[2] == 'D')
				left(res);
			else if (buff[1] == '[' && buff[2] == 'C')
				right(res);
			underline();
		}	
		else if (buff[0] == ' ')
		{
			if (!CURP->selected)
			{
				reverse_video(CURP->str);
				CURP->selected = 1;
				++g_env.nb_selected;
			}
			else
			{
				ft_fputstr(CURP->str, g_env.fd);
				CURP->selected = 0;
				--g_env.nb_selected;
			}
			down(res, 0);
			underline();
		}
		else if (buff[0] == 127 || buff[0] == 126)
		{
			delete_param();
			underline();
		}
		else if (buff[0] == 10)
			return (1);
		else if (buff[0] == 's' && !g_env.search)
			g_env.search = 1;
		else if (g_env.search && ft_isprint(buff[0]))
		{
			free_strjoin(buff, &srch, 0);
			search(srch);
		}
		else if (buff[0] == 1 && !buff[1])
			select_all();
		else if (buff[0] == 23 && !buff[1])
			deselect_all();
	}
	return (0);
}

static void		get_new_window_size(void)
{
	struct winsize	w_size;

	ioctl(0, TIOCGWINSZ, &w_size);
	g_env.screen_height = w_size.ws_row;
	g_env.screen_width = w_size.ws_col;
}

static void		update_window(int first)
{
	term_clear();
	get_new_window_size();
	print_list(first);
}

static void		move_window_event(int sig)
{
	update_window(0);
	(void)sig;
}

static void		stop_term(int sig)
{
	reset_term(1);
	(void)sig;
}

static void		take_back(int sig)
{
	if (tcsetattr(0, TCSADRAIN, &g_env.old_term) == -1)
	{
		ft_putendl("tcsetattr failure");
		exit(-1);
	}
	term_clear();
	print_list(0);
	get_stdin();
	(void)sig;
}

static void		save_term(int sig)
{
	struct termios	term;
	char			cp[2];

	term = g_env.old_term;
	if (tcgetattr(0, &g_env.old_term) == -1)
		ft_putendl("tcgetattr failure");
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		ft_putendl("tcsetattr failure");
	signal(SIGTSTP, SIG_DFL);
	cp[0] = g_env.old_term.c_cc[VSUSP];
	cp[1] = '\0';
	ioctl(0, TIOCSTI, cp);
	term_clear();
	(void)sig;
}

static void		signals_set(void)
{
	signal(SIGABRT, &stop_term);
	signal(SIGALRM, &stop_term);
	signal(SIGBUS, &stop_term);
	signal(SIGFPE, &stop_term);
	signal(SIGHUP, &stop_term);
	signal(SIGILL, &stop_term);
	signal(SIGINT, &stop_term);
	signal(SIGKILL, &stop_term);
	signal(SIGPIPE, &stop_term);
	signal(SIGPROF, &stop_term);
	signal(SIGQUIT, &stop_term);
	signal(SIGSEGV, &stop_term);
	signal(SIGTSTP, &save_term);
	signal(SIGSYS, &stop_term);
	signal(SIGTERM, &stop_term);
	signal(SIGTRAP, &stop_term);
	signal(SIGTSTP, &save_term);
	signal(SIGTTIN, &save_term);
	signal(SIGTTOU, &save_term);
	signal(SIGUSR1, &stop_term);
	signal(SIGUSR2, &stop_term);
	signal(SIGURG, &stop_term);
	signal(SIGVTALRM, &stop_term);
	signal(SIGXCPU, &stop_term);
	signal(SIGXFSZ, &stop_term);
	signal(SIGCONT, &take_back);
	signal(SIGWINCH, &move_window_event);
}

static void		display_selected_param(void)
{
	t_list	*tmp;
	t_param	*param;
	int		i;

	tmp = g_env.params;
	param = tmp->data;
	i = 0;
	if (param->selected)
	{
		ft_putstr(param->str);
		if (++i < g_env.nb_selected)
			ft_putchar(' ');
	}
	tmp = tmp->next;
	while (tmp != g_env.params)
	{
		param = tmp->data;
		if (param->selected)
		{
			ft_putstr(param->str);
			if (++i < g_env.nb_selected)
				ft_putchar(' ');
		}
		tmp = tmp->next;
	}
}

int              main(int ac, char **av)
{
	struct termios	term;
	int				ret;
	char			*name;

	if (ac < 2)
	{
		ft_putendl("No param");
		return (-1);
	}
	init_term(&term);
	signals_set();
	ft_bzero(&g_env, sizeof(g_env));
	get_new_window_size();
	g_env.params = get_params(ac - 1, av);
	g_env.nb_params = ac - 1;
	g_env.nb_selected = 0;
	g_env.search = 0;
	g_env.current_param = g_env.params;
	g_env.old_term = term;
	if (!(isatty(0)) || !(name = ttyname(0)) || ((g_env.fd = open(name, O_WRONLY)) == -1))
		reset_term(1);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
	{
		ft_putendl("tcsetattr failure");
		close(g_env.fd);
		return (-1);
	}
	if (term_clear() == -1)
		reset_term(1);
	print_list(1);
	ret = get_stdin();
	reset_term(0);
	if (ret)
		display_selected_param();
	close(g_env.fd);
	return (0);
}
