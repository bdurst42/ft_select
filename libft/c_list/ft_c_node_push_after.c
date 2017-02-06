/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_c_node_push_after.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 21:59:14 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/06 22:09:25 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void	ft_c_node_push_after(t_list **begin_list, void (*data))
{
	t_list	*new_node;
	t_list	*ptr;

	if (begin_list)
	{
		if (*begin_list)
		{
			ptr = *begin_list;
			new_node = ft_create_node(data);
			if (ptr->next)
			{
				ptr->next->prev = new_node;
				new_node->next = ptr->next;
			}
			ptr->next = new_node;
			new_node->prev = ptr;
		}
		else
			*begin_list = ft_create_node(data);
	}
	return ;
}
