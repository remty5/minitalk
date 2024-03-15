/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 22:31:36 by rvandepu          #+#    #+#             */
/*   Updated: 2024/03/15 03:17:18 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static inline void	usage(int argc, char *argv[])
{
	if (argc < 1)
		return ;
	ft_fprintf(stderr, "Usage: %s <server pid> <message>\n", argv[0]);
}

static inline bool	isascii(char *str)
{
	while (*str)
		if (*str++ < 0)
			return (false);
	return (true);
}

bool	g_ack = false;

static void	ack(int sig)
{
	(void) sig;
	g_ack = true;
}

static inline void	send(int pid, bool bit)
{
	unsigned int	i;

	i = 0;
	if (bit)
		kill(pid, SIGUSR2);
	else
		kill(pid, SIGUSR1);
	while (!g_ack)
		if (++i == 0)
			(ft_fprintf(stderr, "fatal: timeout\n"), exit(EXIT_FAILURE));
	g_ack = false;
}

int	main(int argc, char *argv[])
{
	int				pid;
	size_t			len;
	unsigned char	i;
	bool			ascii;

	if (argc != 3)
		return (usage(argc, argv), EXIT_FAILURE);
	pid = ft_atoi(argv[1]);
	if (pid <= 0 || kill(pid, 0) < 0)
		return (ft_fprintf(stderr, "Invalid pid\n"), EXIT_FAILURE);
	signal(SIGUSR1, ack);
	len = ft_strlen(argv[2]);
	if (!len)
		return (EXIT_SUCCESS);
	send(pid, ascii = isascii(argv[2]));
	i = sizeof(size_t) * 8;
	while (i--)
		send(pid, len >> i & 1);
	while (len--)
	{
		i = 8 - ascii;
		while (i--)
			send(pid, argv[2][len] >> i & 1);
	}
	return (EXIT_SUCCESS);
}
