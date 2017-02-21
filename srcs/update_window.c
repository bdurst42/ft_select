#include "ft_select.h"

static void	increase_range(int i)
{
	while (i--)
		g_e.end = g_e.end->next;
}

static void	decrease_range(int i)
{
	while (i++)
		g_e.end = g_e.end->prev;
}

static void	change_range(int old_max_p)
{
	int				diff;

	if (g_e.nb_p < old_max_p)
		diff = g_e.max_p - g_e.nb_p;
	else
		diff = g_e.max_p - old_max_p;
	if (diff > 0)
		increase_range(diff);
	else if (diff < 0)
		decrease_range(diff);
}

void		get_new_window_size(void)
{
	struct winsize	w_size;
	int				old_max_p;

	ioctl(0, TIOCGWINSZ, &w_size);
	g_e.s_height = w_size.ws_row - 1;
	g_e.s_width = w_size.ws_col;
	old_max_p = g_e.max_p;
	g_e.max_p = g_e.s_height * (g_e.s_width / (g_e.max_len + 1));
	if (g_e.end)
	{
		if (g_e.max_p < g_e.nb_p)
			change_range(old_max_p);
		else
		{
			g_e.start = g_e.params;
			g_e.end = g_e.params->prev;
		}
	}
}

void		update_window(void)
{
	term_clear();
	get_new_window_size();
	if (!g_e.s_height)
		ft_putstr("Window too small !");
	else
		print_list();
}
