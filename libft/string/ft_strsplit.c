/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/27 12:32:59 by bdurst            #+#    #+#             */
/*   Updated: 2016/05/27 12:32:59 by bdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

static size_t	ft_nb_words(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i] && s)
	{
		if (s[i] == c)
			i++;
		else
		{
			j++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (j);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**tab;
	size_t	len;
	size_t	len_word;
	size_t	j;

	if (s && (tab = (char **)malloc(sizeof(char *) * (ft_nb_words(s, c) + 1))))
	{
		j = 0;
		len = 0;
		while (s[len])
		{
			while (s[len] == c)
				len++;
			len_word = len;
			while (s[len] && s[len] != c)
				len++;
			if (len > len_word)
				tab[j++] = ft_strsub(s, len_word, len - len_word);
		}
		tab[j] = NULL;
		return ((char **)tab);
	}
	return (NULL);
}
