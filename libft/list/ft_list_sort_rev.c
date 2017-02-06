/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_sort_rev.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:56 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:56 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	ft_list_sort_rev(t_list **begin_list)
{
	t_list	*node_1;
	t_list	*node_2;

	if (begin_list && *begin_list)
	{
		node_1 = *begin_list;
		node_2 = node_1->next;
		while (node_1)
		{
			node_1->next = node_1->prev;
			node_1->prev = node_2;
			if (node_2)
				node_2 = node_2->next;
			*begin_list = node_1;
			node_1 = node_1->prev;
		}
	}
}
