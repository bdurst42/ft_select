#include "ft_select.h"

void	deselect_all(void)
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

void	select_all(void)
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

void	free_strjoin(char *s1, char **s2, char c)
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
