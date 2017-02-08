/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/12 13:14:16 by bdurst            #+#    #+#             */
/*   Updated: 2017/02/08 21:15:12 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

# include <stdlib.h>
# include <string.h>

typedef struct		s_list
{
	void			*data;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;

t_list				*ft_create_node(void *data);
t_list				*ft_go_start_list(t_list *node);
t_list				*ft_go_end_list(t_list *node);
void				ft_node_push_back(t_list **begin_list, void *data);
void				ft_node_push_front(t_list **begin_list, void *data);
void				ft_node_push_after(t_list **begin_list, void *data);
void				ft_add_list(t_list *begin_list, t_list *new_list);
void				ft_list_sort_rev(t_list **begin_list);
void				ft_bubblesort_list(t_list **begin_list, int (*cmp)
					(void *right, void *left));
void				ft_insertsort_list(t_list **bg_list, void *data,
					int (*cmp)(void *right, void *left));
void				ft_clear_list(t_list **list, void (*fct_free)(void *));
void				ft_pop_node(t_list **node, void (*fct_free)(void *));
int					ft_list_size(t_list *begin_list);

void				ft_c_node_push_back(t_list **begin_list, void *data);
void				ft_c_node_push_front(t_list **begin_list, void *data);
void				ft_c_node_push_after(t_list **begin_list, void *data);
int					ft_c_list_size(t_list *begin_list);

#endif
