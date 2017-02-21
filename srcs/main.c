/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 23:29:23 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/20 17:06:18 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <term.h>

static t_env	g_e;

static void		init_term(struct termios *term)
{
	char	*name_term;

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
	ft_fputchar(c, g_e.fd);
	return (0);
}

static char		term_clear(void)
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
	if (tcsetattr(0, TCSADRAIN, &g_e.old_term) == -1)
	{
		ft_putendl("tcsetattr failure");
		close(g_e.fd);
		exit(-1);
	}
	ft_fputstr(tgetstr("te", NULL), g_e.fd);
	if (out)
	{
		exit(-1);
		close(g_e.fd);
	}
}

static void		get_max_len(int ac, char **av)
{
	int		i;
	int		len;

	i = -1;
	while (++i < ac)
	{
		len = ft_strlen(av[i + 1]);
		if (len > g_e.max_len)
			g_e.max_len = len;
	}
}

static void		compute_pos(t_param *param, int i)
{
	param->col = i / g_e.s_height;
	param->pos.y = i % g_e.s_height;
	param->pos.x = param->col * (g_e.max_len + 1);
}

static t_list	*get_params(int ac, char **av)
{
	int		i;
	t_list	*params;
	t_param *param;

	params = NULL;
	i = -1;
	get_max_len(ac, av);
	while (++i < ac)
	{
		if (!(param = (t_param*)malloc(sizeof(t_param))))
			reset_term(1);
		param->selected = 0;
		param->len = ft_strlen(av[i + 1]);
		param->str = av[i + 1];
		if (i <= g_e.max_p)
			compute_pos(param, i);
		if (i == g_e.max_p)
			g_e.end = params->prev;
		ft_c_node_push_back(&params, param);
	}
	g_e.start = params;
	if (!g_e.end)
		g_e.end = params->prev;
	return (params);
}

static void		get_param_next_at(t_list *elem, int nb_i)
{
	int	i;

	i = -1;
	while (++i < nb_i)
		elem = elem->next;
	g_e.cur_p = elem;
}

static void		get_param_prev_at(t_list *elem, int nb_i)
{
	while (nb_i--)
		elem = elem->prev;
	g_e.cur_p = elem;
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
	g_e.cur_p = elem;
}

static void		reverse_video(char *str)
{
	tputs(tgetstr("mr", NULL), 1, &my_outc);
	ft_fputstr(str, g_e.fd);
	tputs(tgetstr("me", NULL), 1, &my_outc);
}

static void		up(char *res)
{
	if (g_e.nb_p <= g_e.max_p || g_e.cur_p != g_e.start)
	{
		if (CURP->selected)
			reverse_video(CURP->str);
		else
			ft_fputstr(CURP->str, g_e.fd);
		g_e.cur_p = g_e.cur_p->prev;
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	}
}

static void		down(char *res, char arrow)
{
	if (g_e.nb_p <= g_e.max_p || g_e.cur_p != g_e.end)
	{
		if (arrow)
		{
			if (CURP->selected)
				reverse_video(CURP->str);
			else
				ft_fputstr(CURP->str, g_e.fd);
		}
		g_e.cur_p = g_e.cur_p->next;
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	}
}

static void		print_list(void)
{
	t_list	*tmp;
	t_param	*param;
	char	*res;
	int		i;

	res = CM;
	i = -1;
	tmp = g_e.start;
	while (tmp != g_e.end->next || i == -1)
	{
		param = tmp->data;
		compute_pos(param, ++i);
		tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
		if (param->selected)
			reverse_video(param->str);
		else
			ft_fputstr(param->str, g_e.fd);
		tmp = tmp->next;
	}
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		right_shift(void)
{
	t_list	*tmp;
	t_list	*tmpbis;
	t_param	*param;
	t_param	*parambis;
	int		i;

	tmp = g_e.start;
	tmpbis = g_e.start;
	i = -1;
	while (++i < g_e.s_height)
		tmpbis = tmpbis->prev;
	g_e.start = tmpbis;
	while (tmp != g_e.end)
	{
		param = tmp->data;
		parambis = tmpbis->data;
		parambis->col = param->col;
		parambis->pos = param->pos;
		tmp = tmp->next;
		tmpbis = tmpbis->next;
	}
	g_e.end = tmpbis;
	term_clear();
	print_list();
}

static void		left(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_e.fd);
	if (g_e.nb_p > g_e.max_p && !CURP->col)
		right_shift();
	if (CURP->col * g_e.s_height + CURP->pos.y + 1 - g_e.s_height >= 0)
		get_param_prev_at(g_e.cur_p, g_e.s_height);
	else if (g_e.nb_p > g_e.s_height)
		get_param_by_end(g_e.params->prev, CURP->pos.y);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		left_shift(void)
{
	t_list	*tmp;
	t_list	*tmpbis;
	t_param	*param;
	t_param	*parambis;
	int		i;

	tmp = g_e.end;
	tmpbis = g_e.end;
	i = -1;
	while (++i < g_e.s_height)
		tmpbis = tmpbis->next;
	g_e.end = tmpbis;
	while (tmp != g_e.start)
	{
		param = tmp->data;
		parambis = tmpbis->data;
		parambis->col = param->col;
		parambis->pos = param->pos;
		tmp = tmp->prev;
		tmpbis = tmpbis->prev;
	}
	g_e.start = tmpbis;
	term_clear();
	print_list();
}

static void		right(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_e.fd);
	if (g_e.nb_p > g_e.max_p && (CURP->col + 1) == \
		g_e.s_width / (g_e.max_len + 1))
		left_shift();
	if ((CURP->col + 1) * g_e.s_height + CURP->pos.y + 1 <= g_e.nb_p)
		get_param_next_at(g_e.cur_p, g_e.s_height);
	else if ((CURP->col + 1) * g_e.s_height < g_e.nb_p)
		g_e.cur_p = g_e.params->prev;
	else if (g_e.nb_p > g_e.s_height)
		get_param_next_at(g_e.params, CURP->pos.y);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		underline(void)
{
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	if (CURP->selected)
		tputs(tgetstr("mr", NULL), 1, &my_outc);
	tputs(tgetstr("us", NULL), 1, &my_outc);
	ft_fputstr(CURP->str, g_e.fd);
	tputs(tgetstr("ue", NULL), 1, &my_outc);
	if (CURP->selected)
		tputs(tgetstr("me", NULL), 1, &my_outc);
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		compute_max_len(void)
{
	t_list	*tmp;
	t_param	*param;

	tmp = g_e.params;
	param = tmp->data;
	g_e.max_len = 0;
	if (param->len > g_e.max_len)
		g_e.max_len = param->len;
	tmp = tmp->next;
	while (tmp != g_e.params)
	{
		param = tmp->data;
		if (param->len > g_e.max_len)
			g_e.max_len = param->len;
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

	tmp = g_e.end;
	while (tmp != g_e.cur_p)
	{
		param = tmp->data;
		prev_param = tmp->prev->data;
		param->pos.x = prev_param->pos.x;
		param->pos.y = prev_param->pos.y;
		param->col = prev_param->col;
		tmp = tmp->prev;
	}
}

static void		delete_param(void)
{
	t_list	*prev;

	if (CURP->len == g_e.max_len)
		compute_max_len();
	if (g_e.nb_p > g_e.max_p)
		g_e.end = g_e.end->next;
	move_pos();
	prev = g_e.cur_p->prev;
	g_e.cur_p = g_e.cur_p->next;
	if (g_e.cur_p->prev == g_e.params)
		g_e.params = g_e.params->next;
	if (g_e.cur_p->prev == g_e.start)
		g_e.start = g_e.start->next;
	else if (g_e.cur_p->prev == g_e.end && g_e.nb_p <= g_e.max_p)
		g_e.end = g_e.end->prev;
	ft_pop_node(g_e.cur_p->prev, (void (*)(void *))&free_param);
	g_e.cur_p->prev = prev;
	--g_e.nb_p;
	term_clear();
	if (!g_e.nb_p)
		reset_term(1);
	print_list();
}

static void		clear_last_searchs(void)
{
	t_list	*tmp;
	t_param	*param;

	param = NULL;
	tmp = g_e.start;
	while (tmp != g_e.end->next || !param)
	{
		param = tmp->data;
		if (param->selected == 2)
		{
			tputs(tgoto(CM, param->pos.x, param->pos.y), 1, &my_outc);
			ft_fputstr(param->str, g_e.fd);
			param->selected = 0;
			--g_e.nb_selected;
		}
		tmp = tmp->next;
	}
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		find_occurence(t_list *tmp, t_param *param)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_fputstr(CURP->str, g_e.fd);
	g_e.cur_p = tmp;
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	reverse_video(param->str);
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	if (!param->selected)
		param->selected = 2;
	++g_e.nb_selected;
}

static void		search(char *str)
{
	t_list	*tmp;
	t_param	*param;
	int		len;

	clear_last_searchs();
	param = NULL;
	if ((len = ft_strlen(str)) > g_e.max_len)
		return ;
	tmp = g_e.start;
	while (tmp != g_e.end->next || !param)
	{
		param = tmp->data;
		if (!ft_strncmp(str, param->str, len) || \
		(ft_strchr(str, '*') && nmatch(param->str, str)))
			find_occurence(tmp, param);
		tmp = tmp->next;
	}
	underline();
}

static void		deselect_all(void)
{
	t_list	*tmp;
	t_param	*param;

	tmp = g_e.params;
	param = NULL;
	while (tmp != g_e.params || !param)
	{
		param = tmp->data;
		if (param->selected)
		{
			tputs(tgoto(CM, param->pos.x, param->pos.y), 1, &my_outc);
			ft_fputstr(param->str, g_e.fd);
			--g_e.nb_selected;
			param->selected = 0;
		}
		tmp = tmp->next;
	}
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	underline();
}

static void		select_all(void)
{
	t_list	*tmp;
	t_param	*param;

	tmp = g_e.params;
	param = NULL;
	while (tmp != g_e.params || !param)
	{
		param = tmp->data;
		if (!param->selected)
		{
			tputs(tgoto(CM, param->pos.x, param->pos.y), 1, &my_outc);
			reverse_video(param->str);
			++g_e.nb_selected;
			param->selected = 1;
		}
		tmp = tmp->next;
	}
	tputs(tgoto(CM, CURP->pos.x, CURP->pos.y), 1, &my_outc);
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

static int		press_arrows_escape(char *buff, int res)
{
	if (buff[1] == 0)
	{
		if (g_e.search)
			g_e.search = 0;
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
	return (1);	
}

static void		press_space(int res)
{
	if (!CURP->selected)
	{
		reverse_video(CURP->str);
		CURP->selected = 1;
		++g_e.nb_selected;
	}
	else
	{
		ft_fputstr(CURP->str, g_e.fd);
		CURP->selected = 0;
		--g_e.nb_selected;
	}
	down(res, 0);
	underline();
}

static void		press_search(int res, char *buff, char **srch)
{
	int			len;

	if (buff[0] == 's' && !g_e.search)
	{
		*srch = NULL;
		g_e.search = 1;
		tputs(tgoto(res, 0, g_e.s_height), 1, &my_outc);
		ft_putstr("SEARCH MODE: ");
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	}
	else if (g_e.search && ft_isprint(buff[0]))
	{
		free_strjoin(buff, srch, 0);
		tputs(tgoto(res, 0, g_e.s_height), 1, &my_outc);
		ft_putstr("SEARCH MODE: ");
		len = g_e.s_width - ft_strlen(*srch) - 13;
		if (len > 0)
			ft_putstr(*srch);
		else
			ft_putstr(*srch + ft_strlen(*srch) - g_e.s_width + 13);
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
		search(*srch);
	}
}

static int		read_stdin(void)
{
	int				ret;
	char			*res;
	char			buff[4];
	char			*srch;

	res = CM;
	underline();
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		if (buff[0] == 27 && !press_arrows_escape(buff, res))
			return (0);
		else if (buff[0] == ' ')
			press_space(res);
		else if (buff[0] == 127 || buff[0] == 126)
		{
			g_e.search = 0;
			delete_param();
			underline();
		}
		else if (buff[0] == 10)
			return (1);
		else if (buff[0] == 1 && !buff[1])
		{
			g_e.search = 0;
			select_all();
		}
		else if (buff[0] == 23 && !buff[1])
		{
			g_e.search = 0;
			deselect_all();
		}
		else
			press_search(res, buff, &srch);
	}
	return (0);
}

static void		increase_range(int i)
{
	while (i--)
		g_e.end = g_e.end->next;
}

static void		decrease_range(int i)
{
	while (i++)
		g_e.end = g_e.end->prev;
}

static void		change_range(int old_max_p)
{
	int				diff;

	if (g_e.nb_p < old_max_p)
		diff = g_e.max_p - g_e.nb_p;
	else
		diff = g_e.max_p - old_max_p;
	if (diff > 0)
		increase_range(diff);
	else if (diff < 0)
		decrease_range(diff);
}

static void		get_new_window_size(void)
{
	struct winsize	w_size;
	int				old_max_p;

	ioctl(0, TIOCGWINSZ, &w_size);
	g_e.s_height = w_size.ws_row - 1;
	g_e.s_width = w_size.ws_col;
	old_max_p = g_e.max_p;
	g_e.max_p = g_e.s_height * (g_e.s_width / (g_e.max_len + 1));
	if (g_e.end)
	{
		if (g_e.max_p < g_e.nb_p)
			change_range(old_max_p);
		else
		{
			g_e.start = g_e.params;
			g_e.end = g_e.params->prev;
		}
	}
}

static void		update_window(void)
{
	term_clear();
	get_new_window_size();
	if (!g_e.s_height)
		ft_putstr("Window too small !");
	else
		print_list();
}

static void		move_window_event(int sig)
{
	update_window();
	(void)sig;
}

static void		stop_term(int sig)
{
	struct termios	term;
	char			cp;

	if (sig != SIGTTOU && sig != SIGTTIN && sig != SIGTSTP)
		reset_term(1);
	else
	{
		term = g_e.old_term;
		init_term(&g_e.old_term);
		cp = term.c_cc[VSUSP];
		ft_fputstr(tgetstr("te", NULL), g_e.fd);
		if (tcsetattr(0, TCSADRAIN, &term) == -1)
			ft_putendl("tcsetattr failure");
		ioctl(0, TIOCSTI, &cp);
		signal(SIGTSTP, SIG_DFL);
	}
	(void)sig;
}

static void		take_back(int sig)
{
	struct termios	term;

	signals_set();
	ft_fputstr(tgetstr("ti", NULL), g_e.fd);
	init_term(&term);
	g_e.old_term = term;
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
	{
		ft_putendl("tcsetattr failure");
		exit(-1);
	}
	update_window();
	(void)sig;
}

void		signals_set(void)
{
	int i;

	i = 0;
	while (++i < 32)
	{
		if (i == SIGCONT)
			signal(SIGCONT, &take_back);
		else if (i == SIGWINCH)
			signal(SIGWINCH, &move_window_event);
		else
			signal(SIGABRT, &stop_term);
	}
}

static void		display_selected_param(void)
{
	t_list	*tmp;
	t_param	*param;
	int		i;

	tmp = g_e.params;
	param = tmp->data;
	i = 0;
	if (param->selected)
	{
		ft_putstr(param->str);
		if (++i < g_e.nb_selected)
			ft_putchar(' ');
	}
	tmp = tmp->next;
	while (tmp != g_e.params)
	{
		param = tmp->data;
		if (param->selected)
		{
			ft_putstr(param->str);
			if (++i < g_e.nb_selected)
				ft_putchar(' ');
		}
		tmp = tmp->next;
	}
}

void			free_list(void)
{
	t_list	*tmp;

	tmp = g_e.params;
	free(tmp->data);
	free(tmp);
	tmp = tmp->next;
	while (tmp != g_e.params)
	{
		free(tmp->data);
		free(tmp);
		tmp = tmp->next;
	}
	g_e.params = NULL;
}

/*int              main(int ac, char **av)
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
	ft_bzero(&g_e, sizeof(g_e));
	get_new_window_size();
	g_e.params = get_params(ac - 1, av);
	g_e.nb_p = ac - 1;
	g_e.nb_selected = 0;
	g_e.search = 0;
	g_e.cur_p = g_e.params;
	g_e.old_term = term;
	if (!(isatty(0)) || !(name = ttyname(0)) || ((g_e.fd = open(name, O_WRONLY)) == -1))
		reset_term(1);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
	{
		ft_putendl("tcsetattr failure");
		close(g_e.fd);
		return (-1);
	}
	ft_fputstr(tgetstr("ti", NULL), g_e.fd);
	if (term_clear() == -1)
		reset_term(1);
	ft_fputstr(tgetstr("ti", NULL), g_e.fd);
	print_list();
	ret = read_stdin();
	reset_term(0);
	if (ret)
		display_selected_param();
	close(g_e.fd);
	free_list();
	return (0);
}*/
