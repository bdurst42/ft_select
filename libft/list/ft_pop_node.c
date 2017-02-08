/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pop_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 21:07:04 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/08 23:41:02 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
# include "io.h"

void	ft_pop_node(t_list **node, void (*fct_free)(void *))
{
	t_list	*tmp;

	if (node && *node)
	{
		tmp = *node;
		if (tmp->next)
			tmp->next->prev = tmp->prev;
		if (tmp->prev)
			tmp->prev->next = tmp->next;
		(*fct_free)(tmp->data);
		free(tmp);
	}
}
