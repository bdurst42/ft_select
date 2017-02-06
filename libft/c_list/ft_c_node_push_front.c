/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_c_node_push_front.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 22:04:26 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/06 22:14:02 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	ft_node_push_front(t_list **begin_list, void *data)
{
	t_list	*new_node;

	if (begin_list)
	{
		if (*begin_list)
		{
			new_node = ft_create_node(data);
			new_node->next = *begin_list;
			new_node->prev = (*begin_list)->prev;
			(*begin_list)->prev = new_node;
			*begin_list = new_node;
		}
		else
			*begin_list = ft_create_node(data);
	}
	return ;
}
