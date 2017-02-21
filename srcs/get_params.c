#include "ft_select.h"

static void	get_max_len(int ac, char **av)
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

static void	compute_pos(t_param *param, int i)
{
	param->col = i / g_e.s_height;
	param->pos.y = i % g_e.s_height;
	param->pos.x = param->col * (g_e.max_len + 1);
}

t_list		*get_params(int ac, char **av)
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

void		print_list(void)
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
		{
			if (param->selected == 2)
				param->selected = 1;
			reverse_video(param->str);
		}
		else
			ft_fputstr(param->str, g_e.fd);
		tmp = tmp->next;
	}
	tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
}
