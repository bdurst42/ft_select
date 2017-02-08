/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 23:29:23 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/08 16:02:29 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <term.h>

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

static void		reset_term(struct termios *term, char out)
{
	if (tcsetattr(0, TCSADRAIN, term) == -1)
	{
		ft_putendl("tcsetattr failure");
		exit(-1);
	}
	if (out)
		exit(-1);
}

static t_list	*get_params(int ac, char **av, t_env *env)
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
		if (len > env->max_len)
			env->max_len = len;
	}
	i = -1;
	while (++i < ac)
	{
		if (!(param = (t_param*)malloc(sizeof(t_param))))
			reset_term(&env->old_term, 1);
		param->selected = 0;
		param->len = ft_strlen(av[i + 1]);
		param->col = i / env->screen_height;
		param->str = av[i + 1];
		param->pos.y = i % env->screen_height;
		param->pos.x = param->col * (env->max_len + 1);
		ft_c_node_push_back(&params, param);
	}
	return (params);
}

static void		get_param_next_at(t_list *elem, int nb_i, t_env *env)
{
	int	i;

	i = -1;
	while (++i < nb_i)
		elem = elem->next;
	env->current_param = elem;
}

static void		get_param_prev_at(t_list *elem, int nb_i, t_env *env)
{
	while (nb_i--)
		elem = elem->prev;
	env->current_param = elem;
}

static void		get_param_by_end(t_list *elem, int y, t_env *env)
{
	t_param	*param;

	param = elem->data;
	while (param->pos.y > y)
	{
		elem = elem->prev;
		param = elem->data;
	}
	env->current_param = elem;
}

static void		reverse_video(t_env *env)
{
	tputs(tgetstr("mr", NULL), 1, &my_outc);
	ft_putstr(CURP->str);
	tputs(tgetstr("me", NULL), 1, &my_outc);
}

static void		up(t_env *env, char *res)
{
	if (CURP->selected)
		reverse_video(env);
	else
		ft_putstr(CURP->str);
	env->current_param = env->current_param->prev;
	tputs(tgoto(res, CURP->pos.x, ((t_param*)env->current_param->data)->pos.y), 1, &my_outc);
}

static void		down(t_env *env, char *res, char arrow)
{
	if (arrow)
	{
		if (CURP->selected)
			reverse_video(env);
		else
			ft_putstr(CURP->str);
	}
	env->current_param = env->current_param->next;
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		left(t_env *env, char *res)
{
	if (CURP->selected)
		reverse_video(env);
	else
		ft_putstr(CURP->str);
	if (CURP->col * env->screen_height + CURP->pos.y + 1 - env->screen_height >= 0)
		get_param_prev_at(env->current_param, env->screen_height, env);
	else
		get_param_by_end(env->params->prev, CURP->pos.y, env);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		right(t_env *env, char *res)
{
	if (CURP->selected)
		reverse_video(env);
	else
		ft_putstr(CURP->str);
	if ((CURP->col + 1) * env->screen_height + CURP->pos.y + 1 <= env->nb_params)
		get_param_next_at(env->current_param, env->screen_height, env);
	else if ((CURP->col + 1) * env->screen_height < env->nb_params)
		env->current_param = env->params->prev;
	else
		get_param_next_at(env->params, CURP->pos.y, env);
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		underline(t_env *env)
{
	char			*area;
	
	/*tputs(tgoto(tgetstr("cm", &area), 60, 0), 1, &my_outc);
	ft_putstr("x = ");
	ft_putnbr(CURP->pos.x);
	ft_putstr(" || y = ");
	ft_putnbr(CURP->pos.y);
	ft_putstr("\n");*/
	tputs(tgoto(tgetstr("cm", &area), CURP->pos.x, CURP->pos.y), 1, &my_outc);
	if (CURP->selected)
	{
		tputs(tgetstr("mr", &area), 1, &my_outc);
		CURP->selected = 1;
	}
	tputs(tgetstr("us", &area), 1, &my_outc);
	ft_putstr(CURP->str);
	tputs(tgetstr("ue", &area), 1, &my_outc);
	if (CURP->selected)
		tputs(tgetstr("me", &area), 1, &my_outc);
	/*tputs(tgoto(tgetstr("cm", &area), 60, 1), 1, &my_outc);
	ft_putstr("x = ");
	ft_putnbr(CURP->pos.x);
	ft_putstr(" || y = ");
	ft_putnbr(CURP->pos.y);
	ft_putstr("\n");*/
	tputs(tgoto(tgetstr("cm", &area), CURP->pos.x, CURP->pos.y), 1, &my_outc);
}

static void		get_stdin(t_env *env)
{
	int				ret;
	char			*res;
	char			*area;
	char			buff[4];

	res = tgetstr("cm", &area);
	underline(env);
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		if (buff[0] == 27)
		{
			if (buff[1] == 0)
				break ;
			else if (buff[1] == '[' && buff[2] == 'A')
				up(env, res);
			else if (buff[1] == '[' && buff[2] == 'B')
				down(env, res, 1);
			else if (buff[1] == '[' && buff[2] == 'D')
				left(env, res);
			else if (buff[1] == '[' && buff[2] == 'C')
				right(env, res);
			underline(env);
		}	
		else if (buff[0] == ' ')
		{
			if (!CURP->selected)
			{
				reverse_video(env);
				CURP->selected = 1;
			}
			else
			{
				ft_putstr(CURP->str);
				CURP->selected = 0;
			}
			down(env, res, 0);
			underline(env);
		}
	}
}

static void		print_list(t_env env)
{
	t_list	*tmp;
	t_param	*param;
	char	*area;
	char	*res;

	tmp = env.params;
	param = tmp->data;
	res = tgetstr("cm", &area);
	tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
	ft_putstr(param->str);
	tmp = tmp->next;
	while (tmp != env.params)
	{
		param = tmp->data;
		tputs(tgoto(res, param->pos.x, param->pos.y), 1, &my_outc);
		ft_putstr(param->str);
		tmp = tmp->next;
	}
	tputs(tgoto(res, 0, 0), 1, &my_outc);
}

int              main(int ac, char **av)
{
	struct termios	term;
	t_env			env;

	init_term(&term);
	if (ac < 2)
	{
		ft_putendl("No param");
		return (-1);
	}
	ft_bzero(&env, sizeof(env));
	env.screen_height = tgetnum("li");
	env.params = get_params(ac - 1, av, &env);
	env.nb_params = ac - 1;
	env.current_param = env.params;
	env.old_term = term;
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
		reset_term(&env.old_term, 1);
	print_list(env);
	get_stdin(&env);
	reset_term(&env.old_term, 0);
	term_clear();
	return (0);
}
