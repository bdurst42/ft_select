/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmatch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/09/07 22:39:27 by bdurst            #+#    #+#             */
/*   Updated: 2016/12/14 02:51:46 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int		nmatch(char *s1, char *s2)
{
	if (*s2 == '*' && *s1 == '\0')
		return (nmatch(s1, s2 + 1));
	if (*s2 == '*' && *s1 != '\0')
		return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
	if (*s1 == '\0' && *s2 == '\0' && *s2 == *s1)
		return (1);
	if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
		return (nmatch(s1 + 1, s2 + 1));
	return (0);
}
