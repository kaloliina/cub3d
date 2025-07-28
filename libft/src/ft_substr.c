/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:22:03 by khiidenh          #+#    #+#             */
/*   Updated: 2025/07/28 12:32:25 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* Allocates memory with malloc and returns a substring from the string 's'.
The substring begins at index 'start' and is of maximum size 'len'

- s -> The string from which to create the substring.
- start -> The start of index of the substring in the string 's'.
- len -> The maximum length of the substring.

- Returns the substring
- Returns NULL if the allocation fails
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*subs;
	size_t	length;
	size_t	string_length;

	string_length = ft_strlen(s);
	i = 0;
	if (start >= ft_strlen(s))
		return (subs = ft_calloc(1, 1));
	length = ft_strlen(s) - start;
	if (length > len)
		length = len;
	subs = ft_calloc(length + 1, sizeof(char));
	if (subs == NULL)
		return (NULL);
	while (len > 0 && start < string_length)
	{
		subs[i++] = s[start++];
		len--;
	}
	subs[i] = '\0';
	return (subs);
}
