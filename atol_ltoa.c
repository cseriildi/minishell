/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol_ltoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:25:46 by icseri            #+#    #+#             */
/*   Updated: 2024/10/25 11:18:09 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_atoll(const char *str)
{
	int			sign;
	long long	nbr;

	nbr = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nbr = nbr * 10 + *str++ - '0';
	}
	return (sign * nbr);
}

static long long	ft_nbrlen(long long n)
{
	long long	digit;

	digit = 1;
	if (n < 0)
	{
		if (n / -10 == 0)
			return (digit);
		else
		{
			n /= -10;
			digit++;
		}
	}
	while (n / 10 != 0)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

static long long	ft_power(long long nb, long long power)
{
	if (power < 0)
		return (0);
	else if (power == 0)
		return (1);
	else
		return (nb * ft_power(nb, power - 1));
}

char	*ft_lltoa(long long nb)
{
	char		*nbr_as_str;
	long long	dig;
	long long	curr;
	long long	sign;
	long long	i;

	sign = ft_power(-1, (nb < 0));
	dig = ft_nbrlen(nb);
	nbr_as_str = ft_calloc(dig + 1 + (sign == -1), 1);
	if (nbr_as_str == NULL)
		return (NULL);
	i = (sign == -1);
	while (dig-- > 0)
	{
		if (sign == -1)
			nbr_as_str[0] = '-';
		curr = nb / (ft_power(10, dig) * sign);
		nbr_as_str[i++] = curr + '0';
		if (curr == 0)
			curr = 1;
		nb = (nb % (curr * ft_power(10, dig) * sign)) * sign;
		sign *= sign;
	}
	return (nbr_as_str);
}

bool	is_ll_overflow(t_var *data, char *str)
{
	char	*new_str;
	char	*trimmed;
	int		i;
	int		j;

	new_str = ft_lltoa(ft_atoll(str));
	if (!new_str)
		malloc_failed(data);
	trimmed = ft_strtrim(str, WHITE_SPACES);
	if (!trimmed)
	{
		free(new_str);
		malloc_failed(data);
	}
	j = (trimmed[0] == '-' && trimmed[1] && new_str[0] == '-');
	i = (trimmed[0] == '+' || trimmed[0] == '-');
	while (trimmed[i] && trimmed[i + 1] && trimmed[i] == '0')
		i++;
	if (ft_strncmp(new_str + j, trimmed + i, ft_strlen(new_str + j) + 1))
		return (free(trimmed), free(new_str), true);
	return (free(trimmed), free(new_str), false);
}
