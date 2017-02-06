/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_c_list_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 21:59:44 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/06 23:01:26 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
# include "io.h"

int	ft_c_list_size(t_list *begin_list)
{
	int		count;
	t_list	*nbr_node;

	count = 0;
	if (begin_list)
	{
		nbr_node = begin_list->next;
		++count;
		while (nbr_node != begin_list)
		{
			++count;
			nbr_node = nbr_node->next;
		}
		return (count);
	}
	return (0);
}
