/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:56 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:56 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int	ft_list_size(t_list *begin_list)
{
	int		count;
	t_list	*nbr_node;

	count = 0;
	if (begin_list)
	{
		nbr_node = begin_list;
		while (nbr_node)
		{
			count++;
			nbr_node = nbr_node->next;
		}
		return (count);
	}
	return (0);
}
