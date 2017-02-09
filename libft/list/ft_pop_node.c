/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pop_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 21:07:04 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/09 14:19:45 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
# include "io.h"

void	ft_pop_node(t_list *node, void (*fct_free)(void *))
{
	if (node)
	{
		if (node->next)
			node->next->prev = node->prev;
		if (node->prev)
			node->prev->next = node->next;
		(*fct_free)(node->data);
		free(node);
	}
}
