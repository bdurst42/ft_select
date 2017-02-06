/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_c_node_push_back.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 22:02:17 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/06 23:16:04 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
# include "io.h"

void		ft_c_node_push_back(t_list **begin_list, void *data)
{
	t_list	*new_node;
	t_list	*ptr;

	if (begin_list)
	{
		if (*begin_list)
		{
			ptr = *begin_list;
			new_node = ft_create_node(data);
			if (ptr->prev)
				ptr = ptr->prev;
			(*begin_list)->prev = new_node;
			ptr->next = new_node;
			new_node->prev = ptr;
			new_node->next = *begin_list;
		}
		else
			*begin_list = ft_create_node(data);
	}
}
