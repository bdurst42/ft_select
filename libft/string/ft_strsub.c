/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:59 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:59 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*strf;
	size_t	i;

	i = 0;
	strf = (char*)malloc(sizeof(char) * (len + 1));
	if (s && strf)
	{
		while (i < len && start < ft_strlen(s))
		{
			strf[i] = s[start];
			start++;
			i++;
		}
		strf[i] = '\0';
		return (strf);
	}
	return (NULL);
}
