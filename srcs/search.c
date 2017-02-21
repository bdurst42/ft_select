#include "ft_select.h"

static void	clear_last_searchs(void)
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

static void	find_occurence(t_list *tmp, t_param *param)
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

void		search(char *str)
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
