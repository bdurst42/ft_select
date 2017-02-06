/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:55 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:55 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include "list.h"
#include "gnl.h"
#include "io.h"

static char		*gnl_get_line(char *str, char **line, int *clean)
{
	int		i;

	i = 0;
	*clean = 1;
	while (str[i] && str[i] != '\n')
		i++;
	if (!(*line = (char *)malloc(sizeof(char) * (i + 1))))
		ft_exiterror("malloc failure", 0);
	i = -1;
	while (str[++i] && str[i] != '\n')
		(*line)[i] = str[i];
	(*line)[i] = '\0';
	if (str[i] == '\n')
		i++;
	return (&str[i]);
}

int				gnl(int const fd, char **line)
{
	static char		*stock = NULL;
	char			buff[BUFF_SIZE + 1];
	char			*tmp;
	int				ret;
	int				clean;

	clean = 0;
	if (fd < 0 || !line || BUFF_SIZE < 1)
		return (-1);
	ret = BUFF_SIZE;
	while (!(ft_strchr(stock, '\n')) && (ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		tmp = stock;
		if (stock && clean)
			free(stock);
		stock = ft_strjoin(tmp, buff);
	}
	if (ret == -1)
		return (-1);
	if (stock)
		stock = gnl_get_line(stock, line, &clean);
	if (!ret && (!stock || !*stock) && (!line || !*line || !**line))
		return (0);
	return (1);
}
