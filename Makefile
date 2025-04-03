# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sperez-s <sperez-s@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/13 09:43:48 by migonzal          #+#    #+#              #
#    Updated: 2025/03/26 21:07:56 by sperez-s         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### ------ COLORS ------ ###

RED		=	'\033[31m'
GREEN	=	'\033[1;32m'
YELLOW	=	'\033[33m'
BLUE	=	'\033[34m'
PURPLE	=	'\033[1;35m'
CIAN	=	'\033[36m'
WHITE	=	'\033[37m'
NONE	=	'\033[0m'



### ------ VARIABLES ------ ###

SRC_DIR	=	./src/

SRC_DIR_PAR	=	./src/parsing/

SRC_DIR_EXP =	./src/expansor/

SRC_DIR_EXEC = ./src/executor/

SRC_DIR_BUILT = ./src/builtins/

SRCS	=	$(SRC_DIR)main.c				\
			$(SRC_DIR_PAR)parser.c			\
			$(SRC_DIR_PAR)parse_env.c		\
			$(SRC_DIR_PAR)parser_cells.c	\
			$(SRC_DIR_PAR)parser_var.c		\
			$(SRC_DIR_PAR)cmd.c				\
			$(SRC_DIR_PAR)init.c			\
			$(SRC_DIR_PAR)split_minishell.c	\
			$(SRC_DIR_PAR)parse_utils.c		\
			$(SRC_DIR_PAR)quotes_aux.c		\
			$(SRC_DIR_EXP)expansor.c		\
			$(SRC_DIR_EXP)expansor_utils.c	\
			$(SRC_DIR_EXEC)executor.c		\
			$(SRC_DIR_EXEC)executor_single_command.c		\
			$(SRC_DIR_EXEC)executor_complex_command.c		\
			$(SRC_DIR_EXEC)executor_utils.c		\
			$(SRC_DIR_EXEC)pipe_utils.c		\
			$(SRC_DIR_EXEC)redir.c		\
			$(SRC_DIR_EXEC)file_open.c		\
			$(SRC_DIR_EXEC)signal.c		\
			$(SRC_DIR_BUILT)env_utils.c	\
			$(SRC_DIR_BUILT)export_utils.c	\
			$(SRC_DIR_BUILT)export.c		\
			$(SRC_DIR_BUILT)builtins_utils.c	\
			$(SRC_DIR_BUILT)unset.c				\
			$(SRC_DIR_BUILT)cd.c				\
			$(SRC_DIR_BUILT)echo.c				\
			$(SRC_DIR_BUILT)env.c				\
			$(SRC_DIR_BUILT)pwd.c				\
			$(SRC_DIR_BUILT)ft_exit.c			\
			$(SRC_DIR_BUILT)builtins_dist.c		

OBJS		=	$(SRCS:.c=.o)

NAME	=	minishell

LDFLAGS = $(LIBFTNAME)




LIBFT	=	libft/

LIBFTNAME = $(LIBFT)libft.a

INCLUDE	=	-Iinclude






CC	=	gcc 
RM = rm -f

CFLAGS	=  -g -Wall -Wextra -Werror #-fsanitize=address -g3

### ------ REGLAS ------ ###

all: $(NAME)


.c.o:
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFTNAME):
	$(MAKE) -C $(LIBFT)








$(NAME)		:$(OBJS) $(LDFLAGS)
			@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS) -L. -lreadline -o $(NAME)
			@echo $(GREEN) ": All ready to work my pana"


clean		:
				@$(RM) $(OBJS)
				@$(MAKE) -C $(LIBFT) fclean

fclean		:	clean
				$(RM) $(NAME)
				@echo $(RED) "Files deleted my pana"

re		:	fclean all


.PHONY: all clean fclean re
