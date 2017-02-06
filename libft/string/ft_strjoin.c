/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:58 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:58 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

static char	*ft_concat(char const *s1, char const *s2, char *dst)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(s1))
	{
		dst[i] = s1[i];
		i++;
	}
	while (i < ft_strlen(s1) + ft_strlen(s2))
	{
		dst[i] = s2[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	return (dst);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*dst;

	if (s1 || s2)
	{
		if (!s2)
			return (dst = ft_strdup(s1));
		else if (!s1)
			return (dst = ft_strdup(s2));
		else
			dst = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
		if (dst)
			return (ft_concat(s1, s2, dst));
	}
	return (NULL);
}
