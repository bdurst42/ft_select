/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_tab_char.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:56 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:56 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

char	**ft_malloc_tab_char(int width, int height)
{
	char	**tab;
	int		i;

	tab = (char**)malloc(sizeof(char*) * (height + 1));
	tab[height] = NULL;
	i = -1;
	while (++i < height)
	{
		tab[i] = (char*)malloc(sizeof(char) * width + 1);
		tab[i][width] = 0;
		ft_strclr(tab[i]);
	}
	return (tab);
}
