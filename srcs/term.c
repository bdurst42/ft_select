#include "ft_select.h"

int		my_outc(int c)
{
	ft_fputchar(c, g_e.fd);
	return (0);
}

char	term_clear(void)
{
	char *res;

	if ((res = tgetstr("cl", NULL)) == NULL)
		return (0);
	tputs(res, 0, &my_outc);
	return (1);
}

void	reset_term(char out)
{
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

void	reverse_video(char *str)
{
	tputs(tgetstr("mr", NULL), 1, &my_outc);
	ft_fputstr(str, g_e.fd);
	tputs(tgetstr("me", NULL), 1, &my_outc);
}

void	underline(void)
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
