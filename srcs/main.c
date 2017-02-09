/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 23:29:23 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/09 18:09:54 by bdurst           ###   ########.fr       */
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
	ft_putchar(c);
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
	if (tcsetattr(0, TCSADRAIN, &g_env.old_term) == -1)
	{
		ft_putendl("tcsetattr failure");
		exit(-1);
	}
	if (out)
		exit(-1);
}

static t_list	*get_params(int ac, char **av)
{
	int		i;
	t_list	*params;
	t_param *param;
	int		len;

	params = NULL;
	i = -1;
	while (++i < ac)
	{
		len = ft_strlen(av[i + 1]);
		if (len > g_env.max_len)
			g_env.max_len = len;
	}
	i = -1;
	while (++i < ac)
	{
		if (!(param = (t_param*)malloc(sizeof(t_param))))
			reset_term(1);
		param->selected = 0;
		param->len = ft_strlen(av[i + 1]);
		param->col = i / g_env.screen_height;
		param->str = av[i + 1];
		param->pos.y = i % g_env.screen_height;
		param->pos.x = param->col * (g_env.max_len + 1);
		ft_c_node_push_back(&params, param);
	}
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
	ft_putstr(str);
	tputs(tgetstr("me", NULL), 1, &my_outc);
}

static void		up(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_putstr(CURP->str);
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
			ft_putstr(CURP->str);
	}
	g_env.current_param = g_env.current_param->next;
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		left(char *res)
{
	if (CURP->selected)
		reverse_video(CURP->str);
	else
		ft_putstr(CURP->str);
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
		ft_putstr(CURP->str);
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
	{
		tputs(tgetstr("mr", NULL), 1, &my_outc);
		CURP->selected = 1;
	}
	tputs(tgetstr("us", NULL), 1, &my_outc);
	ft_putstr(CURP->str);
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
	tmp = g_env.params;
	param = tmp->data;
	param->col = ++i / g_env.screen_height;
	param->pos.y = i % g_env.screen_height;
	param->pos.x = param->col * (g_env.max_len + 1);
	tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
	if (param->selected)
		reverse_video(param->str);
	else
		ft_putstr(param->str);
	tmp = tmp->next;
	while (tmp != g_env.params)
	{
		param = tmp->data;
		param->col = ++i / g_env.screen_height;
		param->pos.y = i % g_env.screen_height;
		param->pos.x = param->col * (g_env.max_len + 1);
		tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
		if (param->selected)
			reverse_video(param->str);
		else
			ft_putstr(param->str);
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

static int		get_stdin(void)
{
	int				ret;
	char			*res;
	char			buff[4];

	res = tgetstr("cm", NULL);
	underline();
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		if (buff[0] == 27)
		{
			if (buff[1] == 0)
				return (0);
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
				ft_putstr(CURP->str);
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
	}
	return (0);
}

static void		update_window(int sig)
{
	struct winsize	w_size;

	term_clear();
	ioctl(0, TIOCGWINSZ, &w_size);
	g_env.screen_height = w_size.ws_row;
	print_list(0);
	(void)sig;
}

static void		stop_term(int sig)
{
	reset_term(1);
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
	signal(SIGSTOP, &save_term);
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
	signal(SIGWINCH, &update_window);
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

	if (ac < 2)
	{
		ft_putendl("No param");
		return (-1);
	}
	init_term(&term);
	signals_set();
	ft_bzero(&g_env, sizeof(g_env));
	g_env.screen_height = tgetnum("li");
	g_env.params = get_params(ac - 1, av);
	g_env.nb_params = ac - 1;
	g_env.nb_selected = 0;
	g_env.current_param = g_env.params;
	g_env.old_term = term;
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
	{
		ft_putendl("tcsetattr failure");
		return (-1);
	}
	if (term_clear() == -1)
		reset_term(1);
	print_list(1);
	ret = get_stdin();
	reset_term(0);
	term_clear();
	if (ret)
		display_selected_param();
	return (0);
}
