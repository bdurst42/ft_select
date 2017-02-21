/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 23:29:23 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/20 17:06:18 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

t_env	g_e;

void		init_term(struct termios *term)
{
	char	*name_term;

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

static void	display_selected_param(void)
{
	t_list	*tmp;
	t_param	*param;
	int		i;

	tmp = g_e.params;
	param = tmp->data;
	i = 0;
	if (param->selected)
	{
		ft_putstr(param->str);
		if (++i < g_e.nb_selected)
			ft_putchar(' ');
	}
	tmp = tmp->next;
	while (tmp != g_e.params)
	{
		param = tmp->data;
		if (param->selected)
		{
			ft_putstr(param->str);
			if (++i < g_e.nb_selected)
				ft_putchar(' ');
		}
		tmp = tmp->next;
	}
}

static void	free_list(void)
{
	t_list	*tmp;

	tmp = g_e.params;
	free(tmp->data);
	free(tmp);
	tmp = tmp->next;
	while (tmp != g_e.params)
	{
		free(tmp->data);
		free(tmp);
		tmp = tmp->next;
	}
	g_e.params = NULL;
}

static int	init_env(int ac, char **av, struct termios *term)
{
	char	*name;

	signals_set();
	ft_bzero(&g_e, sizeof(g_e));
	get_new_window_size();
	g_e.params = get_params(ac - 1, av);
	g_e.nb_p = ac - 1;
	g_e.nb_selected = 0;
	g_e.search = 0;
	g_e.cur_p = g_e.params;
	g_e.old_term = *term;
	if (!(isatty(0)) || !(name = ttyname(0)) || ((g_e.fd = open(name, O_WRONLY)) == -1))
		reset_term(1);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, term) == -1)
	{
		ft_putendl("tcsetattr failure");
		close(g_e.fd);
		return (-1);
	}
	return (0);
}

int			main(int ac, char **av)
{
	struct termios	term;
	int				ret;

	if (ac < 2)
	{
		ft_putendl("No param");
		return (-1);
	}
	init_term(&term);
	if (init_env(ac, av, &term) == -1)
			  return (-1);
	ft_fputstr(tgetstr("ti", NULL), g_e.fd);
	if (term_clear() == -1)
		reset_term(1);
	print_list();
	ret = read_stdin();
	reset_term(0);
	if (ret)
		display_selected_param();
	close(g_e.fd);
	free_list();
	return (0);
}
