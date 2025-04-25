# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/13 09:43:48 by migonzal          #+#    #+#              #
#    Updated: 2025/04/25 13:26:09 by ampocchi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### ------ COLORS ------ ###
RED		= '\033[31m'
GREEN	= '\033[1;32m'
YELLOW	= '\033[33m'
BLUE	= '\033[34m'
PURPLE	= '\033[1;35m'
CIAN	= '\033[36m'
WHITE	= '\033[37m'
NONE	= '\033[0m'

### ------ VARIABLES ------ ###
# La commande 'find' recherche tous les .c, en ignorant le dossier libft.
SRCS	= $(shell find . -path "./libft" -prune -o -type f -name "*.c" -print)
OBJS	= $(SRCS:.c=.o)
NAME	= minishell
LDFLAGS	= $(LIBFTNAME)

LIBFT	= libft/
LIBFTNAME	= $(LIBFT)libft.a
INCLUDE	= -Iinclude

CC		= cc
RM		= rm -f
CFLAGS	= -g -Wall -Wextra -Werror

### ------ RULES ------ ###
all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFTNAME):
	$(MAKE) -C $(LIBFT)

$(NAME): $(OBJS) $(LIBFTNAME)
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS) -L. -lreadline -o $(NAME)
	@echo $(GREEN) ": All ready to work"

clean:
	@$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT) fclean

fclean: clean
	@$(RM) $(NAME)
	@echo $(RED) "Files deleted"

re: fclean all

.PHONY: all clean fclean re
