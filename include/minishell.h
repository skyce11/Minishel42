/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ampocchi <ampocchi@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 09:45:35 by migonzal          #+#    #+#             */
/*   Updated: 2025/05/10 15:35:47 by ampocchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "executor.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

//VARIABLE GLOBAR
extern int	g_signal;

typedef enum e_exit_code
{
	F_CMD_NOT_FOUND = 127,
	F_NOT_FILE = 130,
	F_QUOTE = 2
}	t_exit_code;

typedef enum e_signal
{
	S_BASE, // señal base
	S_HEREDOC, //entra en el heredox
	S_HEREDOC_END, // finalización del heredox
	S_HEREDOC_INTERRUPTED, //
	S_SIGINT, // Ctrl + C
	S_SIGINT_CMD, // Ctrl + C en medio de un comando
	S_CMD, // Se ejecuta un comando
	S_MINI, // Se ejecuta una mini
	S_QUOTE, // se ejecuta cuando esta entre " " o ' '
	S_CANCEL_EXEC, // Ctrl + D en heredoc
	S_SIZE
}	t_signal;

typedef struct s_redir
{
	char			*file;
	int				type; //0 in 1 out 2 heredoc 3 append
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				*cmd_sep;
	struct s_command	*next;
	char				**args;
	t_redir				*redir;
	int					return_val;

}					t_command;

typedef struct s_pip
{
	char				*cmd_pip;
	struct s_pip		*prev;
	struct s_pip		*next;
}					t_pip;

typedef struct s_tools
{
	char				*arg_str;
	char				**paths;
	char				**envp;
	char				*pwd;
	char				*old_pwd;
	int					exit_status;
	int					reset;
	struct s_env		*env;
	struct s_command	*command;
}					t_tools;

typedef struct s_env
{
	char			*var_name;
	char			*value_var;
	struct s_env	*next;
}					t_env;


void	update_shlvl(t_tools *tools);
void	ft_clean_all(t_tools *tools);
void	cleanse_redir_list(t_redir *redir);

//close_fd.c
void	safe_close(int *fd);
void	close_safe(t_command *curr_command, t_pipes_command	pipes);

//  Parser_cells
t_command		*create_cell(char *cmd_sep);
t_command		*add_cell(t_command *list, char *cmd_sep);
void			print_list(t_command *list);
t_redir			*create_redir_list(char *redir, int *status);

//  Parser_pip
t_pip			*create_pip_cell(char *cmd_pip);
t_pip			*add_pip_cell(t_pip *list, char *cmd_pip, int pos);
void			print_pip_list(t_pip *piplist);
int				pipeinstr(char c, char *str);
void			parse_pip(t_command *list);

// cmd.c
int				validate_pipes(char *str);
char			*first_command(char *str);
int				ft_strlen_first_word(char *str);
char			*arg_quote(char *str);

// init.c
t_command		*init_command(void);
void			free_command(t_command *command);

// split minishell
void			*ft_free_matrix(char **mtx);
char			**split_minishell(char *s, char z);
size_t			count_tokens(char *s, char z);
char			*cross_string(char *s, size_t *i, char z);
char			*get_end_str(char *s, char c, size_t *len, int count, char z);
size_t			count_string(char *s, char z);

// parse_utils
char			*word_dup(char *str, int start, int finish);
char			*word_dup_after(char *s, char c, int i);
char			*word_dup_before(char *s, char c);
int				count_words(const char *str, char c);
char			**list_dup_after(char *s, char c);
int				dup_after_pos(char *s, char c, int num);
char			**parse_args(char *s);
void			ft_free_arr(char **arr);
char			**arrdup(char **arr);
size_t			pos_after_char(char *str, char c);
int				find_match_quote(char *line, int i, int *num_del, int del);
int				count_quotes(char *line);

// Parser
void		parser(t_tools *tools);

// parse_envp
int				find_pwd(t_tools *tools);
char			*find_path(char **envp);
int				parse_envp(t_tools *t_tools);

// parser_var
int				get_char_pos(char *s, char c);
char			*get_left_str(char *str);
char			*get_right_str(char *str);
int				if_var(char *str);
int				if_ass_str(char **table);

// quotes_aux.c
size_t			quotes_lenght(char *str);
char			*delete_quotes(char *str, char c);

// expansor.c
char			*ft_strstr(const char *haystack, const char *needle);
char			*expansor(t_tools *tools);
char			*detect_dollar(t_tools *tools);
int				loop_dollar(t_tools *tools, char **aux, int j);

// expansor_utils.c
size_t			dollar_after(char *str);
size_t			equal_after(char *str);
char			*char_to_str(char c);
int				digit_after_dollar(int j, char *str);
int				after_dollar_lenght(char *str, int j);

// 			BUILTINS
//cd
void			ft_cd(t_command *command, t_tools *tools);
//echo
void			print_lines(int i, char **str, int out);
int				ft_echo(t_command *command);
//env
void			ft_env(t_command *command, t_tools *tools);
//pwd
void			ft_pwd(t_tools *tools);
//export
void			add_or_update_env(t_tools *tools, char *var);
void			ft_export(t_command *command, t_tools *tools);
//unset
void			ft_unset(t_command *command, t_tools *tools);
//exit
int	ft_exit(t_tools *tools);

// env_utils
char			*get_env_name(char *fullenv);
char			*get_env_value(char *fullenv);
void			ft_lstadd_back_env(t_env **lst, t_env *new);
t_env			*ft_lstnew_env(char *name, char *value, int alloc);
void			ft_lst_env_init(t_env **env, char **envp);

//Export_utils
int				ft_strcmp(const char *s1, const char *s2);
int				ft_arraylen(char **array);
void			ft_memfree(void *ptr);
void			add_arg_to_env(char *var, t_tools *tools);

//BUILTING_DISTRIBUTOR
int				is_builtin(t_command *command);
void			ft_builtin(t_command *command, t_tools *tools);

// MINISHELL_LOOP
int				reset_tools(t_tools *tools);
int				init_tools(t_tools *tools);
int				minishell_loop(t_tools *tools);

//  BUILTINS_UTILS
void			ft_free_list(t_env **list);
void			change_path(t_tools *tools);
int				check_valid_identifier(char c);

//  Executor
void			executor(t_tools *tools);
void	run_command(t_command *command, t_tools *tools);

// int				file_open(t_redir *redir);
int		file_open(t_tools *tools, t_redir *redir);

t_pipe			*create_pipe_list(int size);
void			cleanse_pipe_list(t_pipe **first);
int				redir_setup(t_tools *tools, t_command *command);

int				get_command_list_size(t_command *list);
int				fill_command_from_env(t_command *command, t_tools *tools);
int				find_command_in_route(t_command *command, char *path);
void			handle_status(int status, t_tools *tools);
t_pipe			*obtain_related_pipe_from_list(t_pipe *ps,
					unsigned int pos, int is_prev);

int				exec_compound_command(t_tools *tools, unsigned int size);

int				exec_single_command(t_command *command, t_tools *tools);
int				check_quotes(int in_dquote, int in_squote, t_tools *tools);
int				check_child_status(int status, int fd, char *result, t_tools *tools);

// Signal
void			signal_init(void);
void			sigint_handler(int sig);
void			sigquit_handler(int sig);

void			break_heredoc(char *line, int err, t_tools *tools);
int				preprocess_heredoc(t_tools *tools, const char *delimiter);

#endif
