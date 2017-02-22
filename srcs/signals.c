#include "ft_select.h"

static void	move_window_event(int sig)
{
	update_window();
	(void)sig;
}

static void	stop_term(int sig)
{
	struct termios	*term;
	char			cp;

	if (sig != SIGTTOU && sig != SIGTTIN && sig != SIGTSTP)
		reset_term(1);
	else
	{
		term = &g_e.old_term;
		cp = term->c_cc[VSUSP];
		ft_fputstr(tgetstr("te", NULL), g_e.fd);
		if (tcsetattr(0, TCSADRAIN, term) == -1)
			ft_putendl("tcsetattr failure");
		ioctl(0, TIOCSTI, &cp);
		signal(SIGTSTP, SIG_DFL);
	}
	(void)sig;
}

static void	take_back(int sig)
{
	struct termios	term;

	signals_set();
	ft_fputstr(tgetstr("ti", NULL), g_e.fd);
	init_term(&term);
	term = g_e.old_term;
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
	{
		ft_putendl("tcsetattr failure");
		exit(-1);
	}
	update_window();
	(void)sig;
}

void		signals_set(void)
{
	int i;

	i = 0;
	while (++i < 32)
	{
		if (i == SIGCONT)
			signal(i, &take_back);
		else if (i == SIGWINCH)
			signal(i, &move_window_event);
		else
			signal(i, &stop_term);
	}
}
