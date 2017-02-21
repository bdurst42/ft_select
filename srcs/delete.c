#include "ft_select.h"

static void	compute_max_len(void)
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

static void	free_param(t_param *param)
{
	if (param)
		free(param);
}

static void	move_pos(void)
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

void		delete_param(void)
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
