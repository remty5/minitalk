# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/06 16:11:02 by rvandepu          #+#    #+#              #
#    Updated: 2024/03/15 04:30:07 by rvandepu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BINARIES := server client

CFLAGS += -Wall -Wextra -Werror

# https://github.com/remty5/libft
LIBFT_DIR	:= libft
LIBFT		:= libft.a
LIBFT_PATH	:= $(LIBFT_DIR)/$(LIBFT)
CPPFLAGS	+= -I$(LIBFT_DIR)
LDFLAGS		+= -L$(LIBFT_DIR)
LDLIBS		+= -lft

.PHONY: all clean fclean re test

all: $(BINARIES)

clean:
	$(MAKE) -s -C $(LIBFT_DIR) fclean
	$(RM) $(BINARIES:%=%.o)

fclean: clean
	$(RM) $(BINARIES)

re: fclean all

test: CFLAGS += -g
test: re

$(LIBFT_PATH):
	$(MAKE) -s -C $(LIBFT_DIR) $(LIBFT) -j $$(nproc)

$(BINARIES): | $(LIBFT_PATH)
