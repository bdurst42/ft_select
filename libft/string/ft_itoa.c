/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:57 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:57 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

static	long	ft_intlen(long n)
{
	long		i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

static	char	*charnbr(char *dst, long i, long k, long n)
{
	while (--i >= 0)
	{
		if (k < 0)
			dst[i] = -(k % 10) + '0';
		else
			dst[i] = k % 10 + '0';
		k = k / 10;
	}
	if (n < 0)
		return (--dst);
	return (dst);
}

char			*ft_itoa(long n)
{
	char		*dst;
	long		i;
	long		k;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	k = n;
	i = ft_intlen(n);
	if (n < 0)
	{
		dst = malloc(sizeof(char) * (i + 2));
		*dst++ = '-';
	}
	else
		dst = malloc(sizeof(char) * (i + 1));
	dst[i] = '\0';
	return (charnbr(dst, i, k, n));
}
