/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 22:30:43 by rvandepu          #+#    #+#             */
/*   Updated: 2024/03/14 09:59:34 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static inline void	read_char(int sig, unsigned char *bpc,
								size_t *len, char *str)
{
	static unsigned char	i;
	static unsigned char	c;

	//printf("[CHAR] i:%d, bpc: %d, c:%x, len:%zu\n", i, *bpc, c, *len);
	if (!i)
		i = *bpc;
	c = c << 1 | (sig == SIGUSR2);
	if (!--i)
	{
		str[--*len] = c;
		c = '\0';
	}
}

static inline void	print_reset(size_t *i, unsigned char *bpc, char **str)
{
	ft_printf("%s", *str);
	free(*str);
	*i = sizeof(size_t) * 8;
	*bpc = 0;
	*str = NULL;
}

static inline void	handler(int sig)
{
	static size_t			i = sizeof(size_t) * 8;
	static unsigned char	bpc;
	static size_t			len;
	static char				*str;

	//printf("[START] sig:%d, i:%zu, bpc:%d, len:%zu, str:%p\n", sig, i, bpc, len, str);
	if (!bpc)
	{
		//ft_printf("read bpc: %d\n", 8 - (sig == SIGUSR2));
		return ((void)(bpc = 8 - (sig == SIGUSR2)));
	}
	if (!str)
	{
		//ft_printf("reading len\n");
		len = len << 1 | (sig == SIGUSR2);
	}
	if (!str && !--i)
	{
		//ft_printf("malloc: %d\n", len);
		return ((void)(str = ft_calloc(1, len + 1)));
	}
	if (str && len)
		read_char(sig, &bpc, &len, str);
	if (str && !len)
		print_reset(&i, &bpc, &str);
}


// TODO redo collision detection
void	receive(int sig, siginfo_t *info, void *ucontext)
{
	(void) ucontext;
	//if (!info->si_pid)
	//	return (ft_fprintf(stderr, "fatal: collision detected\n"),
	//		exit(EXIT_FAILURE));
	handler(sig);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	static struct sigaction	act = (struct sigaction){
		.sa_sigaction = receive, .sa_flags = SA_SIGINFO};

	ft_printf("%d\n", getpid());
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGUSR1);
	sigaddset(&act.sa_mask, SIGUSR2);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	while (true)
		pause();
	return (EXIT_SUCCESS);
}
