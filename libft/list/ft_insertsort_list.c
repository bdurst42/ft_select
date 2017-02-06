/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertsort_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:56 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:56 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

static void	ft_swap(t_list **tmp, t_list **new_node, t_list **bg_list, int i)
{
	(*new_node)->next = *tmp;
	(*new_node)->prev = (*tmp)->prev;
	if ((*tmp)->prev)
		(*tmp)->prev->next = *new_node;
	(*tmp)->prev = *new_node;
	if (!i)
		*bg_list = *new_node;
}

static void	ft_swap_end(t_list **tmp, t_list **new_node)
{
	(*new_node)->next = (*tmp)->next;
	(*new_node)->prev = *tmp;
	(*tmp)->next = *new_node;
}

void		ft_insertsort_list(t_list **bg_list, void *data, int (*cmp)
								(void *left, void *right))
{
	t_list	*tmp;
	t_list	*new_node;
	int		i;

	new_node = ft_create_node(data);
	if (bg_list)
	{
		if (*bg_list)
		{
			i = 0;
			tmp = *bg_list;
			while ((*cmp)(new_node->data, tmp->data) > 0 && tmp->next)
			{
				tmp = tmp->next;
				i++;
			}
			if ((*cmp)(new_node->data, tmp->data) < 0)
				ft_swap(&tmp, &new_node, bg_list, i);
			else
				ft_swap_end(&tmp, &new_node);
		}
		else
			*bg_list = new_node;
	}
}
