/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 22:30:43 by rvandepu          #+#    #+#             */
/*   Updated: 2024/03/15 03:19:36 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static inline void	read_char(int sig, unsigned char *bpc,
								size_t *len, char *str)
{
	static unsigned char	i;
	static unsigned char	c;

	if (!i)
		i = *bpc;
	c = c << 1 | (sig == SIGUSR2);
	if (!--i)
	{
		str[--*len] = c;
		c = '\0';
	}
}

static inline void	handler(int sig, pid_t pid)
{
	static t_decode	data = (t_decode){.i = sizeof(size_t) * 8};

	if (!data.bpc)
		return ((void)(data.lastpid = pid, data.bpc = 8 - (sig == SIGUSR2)));
	if (pid != data.lastpid)
	{
		if (data.str)
			free(data.str);
		ft_fprintf(stderr, "fatal: collision detected\n");
		exit(EXIT_FAILURE);
	}
	if (!data.str)
		data.len = data.len << 1 | (sig == SIGUSR2);
	if (!data.str && !--data.i)
		return ((void)(data.str = ft_calloc(1, data.len + 1)));
	if (data.str && data.len)
		read_char(sig, &data.bpc, &data.len, data.str);
	if (data.str && !data.len)
	{
		ft_printf("%s", data.str);
		free(data.str);
		data = (t_decode){.i = sizeof(size_t) * 8};
	}
}

void	receive(int sig, siginfo_t *info, void *ucontext)
{
	(void) ucontext;
	handler(sig, info->si_pid);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	static const struct sigaction	act = (struct sigaction){
		.sa_sigaction = receive, .sa_flags = SA_SIGINFO};

	ft_printf("%d\n", getpid());
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	while (true)
		pause();
	return (EXIT_SUCCESS);
}
