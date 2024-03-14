/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 22:31:36 by rvandepu          #+#    #+#             */
/*   Updated: 2024/03/14 08:19:05 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static inline bool	isascii(char *str)
{
	while (*str)
		if (*str++ < 0)
			return (false);
	return (true);
}

bool	g_ack;

static inline void	send(int pid, bool bit)
{
	//ft_printf("sending %d\n", bit);
	if (bit)
		kill(pid, SIGUSR2);
	else
		kill(pid, SIGUSR1);
	while (!g_ack)
		;
	g_ack = false;
}

static void	ack(int sig)
{
	(void) sig;
	g_ack = true;
}

static void	usage(int argc, char *argv[])
{
	if (argc < 1)
		return ;
	ft_fprintf(stderr, "Usage: %s <server pid> <message>\n", argv[0]);
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
	//printf("sending length: %zu\n", len);
	i = sizeof(size_t) * 8;
	while (i--)
		send(pid, len >> i & 1);
	//printf("sending message: %s\n", argv[2]);
	while (len--)
	{
		i = 8 - ascii;
		while (i--)
			send(pid, argv[2][len] >> i & 1);
	}
	return (EXIT_SUCCESS);
}
