/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:47:52 by oeddamou          #+#    #+#             */
/*   Updated: 2025/09/14 20:18:07 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_size(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*p;
	int		i;
	int		s;

	i = ft_size(n);
	p = malloc (i + 1);
	if (!p)
		return (NULL);
	p[i] = '\0';
	s = 1;
	if (n < 0)
	{
		p[0] = '-';
		s = -1;
	}
	if (n == 0)
		p[0] = '0';
	while (i > 0 && (p[i - 1] != '-' || n > 0))
	{
		p[i - 1] = (n % 10) * s + '0';
		n = n / 10;
		i--;
	}
	return (p);
}
