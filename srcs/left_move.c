#include "ft_select.h"

static void	get_param_prev_at(t_list *elem, int nb_i)
{
	while (nb_i--)
		elem = elem->prev;
	g_e.cur_p = elem;
}

static void	get_param_by_end(t_list *elem, int y)
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

static void	right_shift(void)
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

void		left(char *res)
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
