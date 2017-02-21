#include "ft_select.h"

static void	get_param_next_at(t_list *elem, int nb_i)
{
	int	i;

	i = -1;
	while (++i < nb_i)
		elem = elem->next;
	g_e.cur_p = elem;
}

static void	left_shift(void)
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

void		right(char *res)
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
