/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 23:01:37 by rvandepu          #+#    #+#             */
/*   Updated: 2024/03/15 03:19:59 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <stdbool.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>

# include "libft.h"

typedef struct s_decode
{
	pid_t			lastpid;
	size_t			i;
	unsigned char	bpc;
	size_t			len;
	char			*str;
}	t_decode;

#endif
