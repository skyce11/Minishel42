# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/27 10:41:27 by migonzal          #+#    #+#              #
#    Updated: 2025/05/03 18:35:48 by ampocchi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #





SRCS =  ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	   	ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
	   	ft_putnbr_fd.c ft_putstr_fd.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
	   	ft_strlen.c ft_strncmp.c ft_strrchr.c ft_substr.c ft_tolower.c ft_toupper.c ft_strnstr.c ft_strtrim.c \
	   	ft_split.c ft_itoa.c \
	   	ft_strmapi.c ft_striteri.c \
		get_next_line.c get_next_line_utils.c ft_strcat.c ft_strndup.c

SRCSB = ft_lstnew_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstiter_bonus.c \
		ft_lstmap_bonus.c




OBJS = ${SRCS:.c=.o}

OBJSB = ${SRCSB:.c=.o}

HDRS = libft.h

NAME = libft.a

CC = cc
RM = rm -f

CFLAGS = -Wall -Wextra -Werror

%.o: %.c ${HDRS}
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
		@ar rc ${NAME} ${OBJS}
		@ranlib ${NAME}

all:	${NAME}

clean:
	@${RM} ${OBJS} ${OBJSB}

fclean: clean
	@${RM} ${NAME} bonus

re:	fclean all

bonus: ${OBJSB}
		@ar rc ${NAME} ${OBJSB}
		@ranlib ${NAME}

.PHONY: 	all clean fclean re


