/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bubblesort_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:55 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:55 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	ft_bubblesort_list(t_list **begin_list,
		int (*cmp)(void *right, void *left))
{
	t_list	*tmp;
	void	*buf;
	int		i;

	if (begin_list && *begin_list)
	{
		tmp = *begin_list;
		while (tmp && tmp->next)
		{
			i = 0;
			if ((*cmp)(tmp->data, tmp->next->data) > 0)
			{
				buf = tmp->data;
				tmp->data = tmp->next->data;
				tmp->next->data = buf;
				i++;
			}
			else
				tmp = tmp->next;
			if (i)
				tmp = *begin_list;
		}
	}
}
