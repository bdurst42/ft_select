#include "ft_select.h"

void		up(char *res)
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

void		down(char *res, char arrow)
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