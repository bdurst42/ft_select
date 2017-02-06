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

static t_list	*get_params(int ac, char **av)
{
	int		i;
	t_list	*params;

	i = 0;
	params = NULL;
	while (++i < ac)
		ft_c_node_push_back(&params, av[i]);
	t_list *tmp;
	tmp = params;
		ft_putendl((char*)tmp->data);
		tmp = tmp->next;
	while (tmp != params)
	{
		ft_putendl((char*)tmp->data);
		tmp = tmp->next;
	}
	ft_putstr("\n");
	return (params);
}

int              main(int ac, char **av)
{
	struct termios	term;
	struct termios	old_term;
	char			buff[4];
	int				ret;
	char			*res;
	char			*area;
	int				col;
	int				raw;
	t_list			*params;

	init_term(&term);
	if (ac < 2)
	{
		ft_putendl("No param");
		return (-1);
	}
	params = get_params(ac, av);
	old_term = term;
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
		reset_term(&old_term, 1);
	raw = 0;
	col = 0;
	while ((ret = read(0, buff, 3)))
	{
		buff[ret] = 0;
		res = tgetstr("cm", &area);
		if (buff[0] == 27)
		{
			if (buff[1] == 0)
				break ;
			else if (buff[1] == '[' && buff[2] == 'A')
			{
				raw -= 1;
				tputs(tgoto(res, col, raw), 1, &my_outc);
			}
			else if (buff[1] == '[' && buff[2] == 'B')
			{
				raw += 1;
				tputs(tgoto(res, col, raw), 1, &my_outc);
			}
			else if (buff[1] == '[' && buff[2] == 'D')
			{
				col -= 1;
				tputs(tgoto(res, col, raw), 1, &my_outc);
			}
			else if (buff[1] == '[' && buff[2] == 'C')
			{
				col += 1;
				tputs(tgoto(res, col, raw), 1, &my_outc);
			}
		}	
	}
	reset_term(&old_term, 0);
	term_clear();
	return (0);
}
