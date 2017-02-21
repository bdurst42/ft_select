#include "ft_select.h"

static int	press_arrows_escape(char *buff, char *res)
{
	if (buff[1] == 0)
	{
		if (g_e.search)
		{
			g_e.search = 0;
			term_clear();
			print_list();
		}
		else
			return (0);
	}
	else if (buff[1] == '[' && buff[2] == 'A')
		up(res);
	else if (buff[1] == '[' && buff[2] == 'B')
		down(res, 1);
	else if (buff[1] == '[' && buff[2] == 'D')
		left(res);
	else if (buff[1] == '[' && buff[2] == 'C')
		right(res);
	underline();
	return (1);	
}

static void	press_space(char *res)
{
	if (!CURP->selected)
	{
		reverse_video(CURP->str);
		CURP->selected = 1;
		++g_e.nb_selected;
	}
	else
	{
		ft_fputstr(CURP->str, g_e.fd);
		CURP->selected = 0;
		--g_e.nb_selected;
	}
	down(res, 0);
	underline();
}

static void	press_search(char *res, char *buff, char **srch)
{
	int			len;

	if (buff[0] == 's' && !g_e.search)
	{
		*srch = NULL;
		g_e.search = 1;
		tputs(tgoto(res, 0, g_e.s_height), 1, &my_outc);
		ft_putstr("SEARCH MODE: ");
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
	}
	else if (g_e.search && ft_isprint(buff[0]))
	{
		free_strjoin(buff, srch, 0);
		tputs(tgoto(res, 0, g_e.s_height), 1, &my_outc);
		ft_putstr("SEARCH MODE: ");
		len = g_e.s_width - ft_strlen(*srch) - 13;
		if (len > 0)
			ft_putstr(*srch);
		else
			ft_putstr(*srch + ft_strlen(*srch) - g_e.s_width + 13);
		tputs(tgoto(res, CURP->pos.x, CURP->pos.y), 1, &my_outc);
		search(*srch);
	}
}

static void	other_keys(char *buff)
{
	if (buff[0] == 127 || buff[0] == 126)
	{
		g_e.search = 0;
		delete_param();
		underline();
	}
	else if (buff[0] == 1 && !buff[1])
	{
		g_e.search = 0;
		select_all();
	}
	else if (buff[0] == 23 && !buff[1])
	{
		g_e.search = 0;
		deselect_all();
	}
}

int			read_stdin(void)
{
	int			ret;
	char			*res;
	char			buff[4];
	char			*srch;

	res = CM;
	underline();
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		if (buff[0] == 27 && !press_arrows_escape(buff, res))
			return (0);
		else if (buff[0] == ' ')
			press_space(res);
		else if (buff[0] == 10)
			return (1);
		else if ((buff[0] == 's' && !g_e.search) || (g_e.search && ft_isprint(buff[0])))
			press_search(res, buff, &srch);
		else
			other_keys(buff);
	}
	return (0);
}
